#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <cstring>
#include <boost/log/trivial.hpp>
#include "Connection.hpp"
#include "../movie/Resolution.hpp"
#include "protocol/Constants.hpp"

const unsigned int MAX_DATAGRAM_SIZE = 1000;

Connection::Connection(boost::asio::io_context& io_context, unique_ptr<MovieLayer>& movie_layer)
    : movie_layer_(movie_layer), tcp_socket_(io_context), udp_socket_(io_context),
      streamed_movie_(nullptr) {

    QObject::connect(this, &Connection::server_closes, this, &Connection::server_close_btn_clicked);
}

tcp::socket& Connection::get_tcp_socket() {
    return tcp_socket_;
}

udp::socket& Connection::get_udp_socket() {
    return udp_socket_;
}

void Connection::start() {
    BOOST_LOG_TRIVIAL(info) << "Connection::start() is doing nothing for now" ;
    BOOST_LOG_TRIVIAL(info) << "UNCOMMENT signal emiting" ;
    /*
    unsigned const short port_num_len = 5;
    boost::array<char, port_num_len> port_num_buf;
    try {
        boost::asio::read(tcp_socket_, boost::asio::buffer(port_num_buf, port_num_buf.size()));
    } catch (boost::system::system_error& err) {
        BOOST_LOG_TRIVIAL(info) << "Error during Connection::start() when reading from tcp_socket: " << err.what() << ". Terminating" ;
        return;
    }



    // Parse port num and set it
    unsigned short client_udp_port = std::stoi(std::string(port_num_buf.data(), port_num_buf.size()));
    BOOST_LOG_TRIVIAL(info) << "UDP Port: " << client_udp_port ;
    try {
        udp_socket_.connect(udp::endpoint(tcp_socket_.remote_endpoint().address(), client_udp_port));
    } catch (std::exception& err) {
        BOOST_LOG_TRIVIAL(info) << "Error in udp_socket connect" ;
    }

    // Send server udp port num
    unsigned short server_udp_port = udp_socket_.local_endpoint().port();
    std::string server_port_num_str = std::to_string(server_udp_port);
    try {
        boost::asio::write(tcp_socket_, boost::asio::buffer(server_port_num_str, server_port_num_str.size()));
    } catch (boost::system::system_error& err) {
        BOOST_LOG_TRIVIAL(info) << "Error during Connection::start() when writing to tcp_socket: " << err.what() << ". Terminating" ;
        return;
    }
    */

    id_string_ = tcp_socket_.remote_endpoint().address().to_string() + " " + std::to_string(tcp_socket_.remote_endpoint().port());
    //emit user_connects(id_string);

    read();
}

void Connection::read() {
    bool is_client_ok = true;

    while(is_client_ok) {

        try {
            read_with_confirmation();
        } catch (boost::system::system_error& err) {
            if (err.code().value() == boost::system::errc::no_such_file_or_directory) {
                BOOST_LOG_TRIVIAL(info) << "Client has disconnected: " << err.what() <<  std::endl;
                emit user_disconnects(id_string_);
                return;
            }
        } catch (std::exception& err) {
            BOOST_LOG_TRIVIAL(info) << "Error during read_with_confirmation. Terminating connection.";
            return;
        }

        BOOST_LOG_TRIVIAL(info) << "Received header is: " << message_.get_header().encode() ;

        auto msg_type = message_.get_header().get_msg_type();
        switch (msg_type) {
        case protocol::GET_MOVIE_LIST:
            is_client_ok = handle_get_movie_list();
            break;
        case protocol::GET_MOVIE:
            is_client_ok = handle_get_movie();
            break;
        case protocol::GET_FRAME:
            is_client_ok = handle_get_frame();
            break;
        case protocol::MOVIE_FINISHED:
            is_client_ok = handle_movie_finished();
            break;
        case protocol::DISCONNECT:
            is_client_ok = false;
            break;
        default:
            BOOST_LOG_TRIVIAL(info) << "Unrecognized header";
            break;
        }

    }

    disconnect_client();
}

bool Connection::handle_get_movie_list() {
    bool is_client_ok = true;
    string movie_list = get_movie_list(*movie_layer_);
    protocol::message_type msg_type = protocol::GIVE_MOVIE_LIST;

    message_.get_header().set_msg_type(msg_type);
    message_.get_header().set_body_len(movie_list.size());
    message_.set_header(message_.get_header().encode());
    message_.set_body(movie_list);

    try {
        send_with_confirmation(tcp_socket_, msg_type);
    } catch (std::exception& err) {
        BOOST_LOG_TRIVIAL(info) << "Error during send_with_confirmation in handle_get_movie_list: " << err.what() ;
        is_client_ok = false;
    }

    return is_client_ok;
}

bool Connection::handle_get_movie() {
    std::string movie_name = std::string(message_.body().get(), message_.get_header().get_body_len());
    streamed_movie_ = make_unique<Movie>(movie_layer_->get_movie_location(movie_name));

    unique_ptr<Frame> frame;
    unsigned int numOfFrames;
    protocol::message_type msg_type = protocol::GIVE_FRAME;
    try {
       frame = make_unique<Frame>(streamed_movie_->videoStream()
                                .getFrame(0)
                                .resize(Resolution::get360p()));

       numOfFrames = streamed_movie_->videoStream().get_num_of_frames();
    } catch (std::out_of_range& err) {
        BOOST_LOG_TRIVIAL(info) << "No such movie on server. Exiting" ;
        return false;
    }

    message_.get_header().set_msg_type(msg_type);
    message_.get_header().set_body_len(frame->data_length());
    message_.get_header().set_num_of_cols(frame->getSize().width);
    message_.get_header().set_num_of_rows(frame->getSize().height);
    message_.get_header().set_frame_num(0);
    message_.get_header().set_num_of_frames(numOfFrames);
    message_.get_header().set_is_key_frame(frame->is_key_frame());
    message_.set_header(message_.get_header().encode());
    message_.set_body(frame->data());

    try {
        send_msg_with_frame(*(frame.get()), msg_type);
    } catch (std::exception& err) {
        BOOST_LOG_TRIVIAL(info) << "Error in Connection::handle_get_movie() during send_msg_with_frame: " << err.what() ;
        return false;
    }

    frame.reset(nullptr);
    return true;
}

bool Connection::handle_get_frame() {
    unique_ptr<Frame> frame;
    unsigned num_of_frames;
    protocol::Header& header = message_.get_header();
    protocol::message_type msg_type = protocol::message_type::GIVE_FRAME;

    try {
        frame = make_unique<Frame>(streamed_movie_->videoStream()
                                .getFrame(message_.get_header().get_frame_num())
                                .resize(Resolution::get360p()));

        num_of_frames = streamed_movie_->videoStream().get_num_of_frames();
    } catch (std::exception& err) {
        BOOST_LOG_TRIVIAL(info) << "An error occured during frame data retrive in handle_get_frame:" << err.what() ;
        BOOST_LOG_TRIVIAL(info) << "Disconnecting user" ;
        return false;
    }

    /*
     * No need to set frame number
     * Frame number: provided by user in request
     */
    message_.get_header().set_msg_type(msg_type);
    message_.get_header().set_body_len(frame->data_length());
    message_.get_header().set_num_of_cols(frame->getSize().width);
    message_.get_header().set_num_of_rows(frame->getSize().height);
    // TODO: If client will always send num of frames then server don't need to set this field
    message_.get_header().set_is_key_frame(frame->is_key_frame());
    message_.get_header().set_num_of_frames(num_of_frames);
    message_.set_header(message_.get_header().encode());
    message_.set_body(frame->data());

    try {
        send_msg_with_frame(*(frame.get()), msg_type);
    } catch (std::exception& err) {
        BOOST_LOG_TRIVIAL(info) << "Error in Connection::handle_get_frame() during send_msg_with_frame: " << err.what() ;
        return false;
    }
    return true;
}

void Connection::send_msg_with_frame(const Frame& frame, protocol::message_type msg_type) {

    if (frame.is_key_frame()) {
        send_with_confirmation(tcp_socket_, msg_type);
    } else {
        send_with_confirmation(udp_socket_, msg_type);
    }
}

void Connection::disconnect_client() {
    emit user_disconnects(id_string_);



    if(tcp_socket_.is_open()) {
        tcp_socket_.get_io_context().post([this]() {
            try {
                tcp_socket_.shutdown(tcp::socket::shutdown_both);
                tcp_socket_.close();
            } catch (std::exception& err) {
                BOOST_LOG_TRIVIAL(info) << "Exception during TCP socket shutdown and close: " << err.what() ;
            }
        });
    }

    if(udp_socket_.is_open()) {
        udp_socket_.get_io_context().post([this]() {
            try {
                udp_socket_.shutdown(udp::socket::shutdown_both);
                udp_socket_.close();
            } catch (std::exception& err) {
                BOOST_LOG_TRIVIAL(info) << "Exception during UDP socket shutdown and close: " << err.what() ;
            }
        });
    }
}

bool Connection::handle_movie_finished() {
    return true;
}

protocol::message_type Connection::read_confirmation() {
    char confirmation[protocol::FieldLength::msg_type + 1] = "";

    boost::asio::read(tcp_socket_, boost::asio::buffer(confirmation, protocol::FieldLength::msg_type));

    auto confirmation_num = std::stoi(std::string(confirmation));
    return static_cast<protocol::message_type>(confirmation_num);
}

void Connection::send_confirmation(protocol::message_type msg_type) {
    std::string confirmation = "0" + to_string(msg_type);
    boost::asio::write(tcp_socket_, boost::asio::buffer(confirmation.c_str(), protocol::FieldLength::msg_type));
}

bool Connection::is_confirmation_correct(protocol::message_type msg_type, protocol::message_type confirmation) {
    return msg_type == confirmation;
}

unsigned int Connection::read_header() {
    return boost::asio::read(tcp_socket_, boost::asio::buffer(message_.data().get(), protocol::HEADER_LENGTH));
}

void Connection::read_body() {
    boost::asio::read(tcp_socket_, boost::asio::buffer(message_.body().get(), message_.get_header().get_body_len()));
}

void Connection::send_header() {
    std::string header(message_.data().get(), protocol::HEADER_LENGTH);
    // BOOST_LOG_TRIVIAL(info) << "Header to send: " << header ;
    BOOST_LOG_TRIVIAL(info) << "Header to send: " << header ;

    boost::asio::write(tcp_socket_, boost::asio::buffer(message_.data().get(), protocol::HEADER_LENGTH));
}

void Connection::send_body(udp::socket& udp_socket) {
    unsigned num_of_full_packets = (message_.msg_len() - protocol::HEADER_LENGTH) / MAX_DATAGRAM_SIZE;

    for (unsigned short i = 0; i < num_of_full_packets; ++i) {
        udp_socket.send(boost::asio::buffer(message_.body().get() + MAX_DATAGRAM_SIZE * i, MAX_DATAGRAM_SIZE));
    }

    unsigned last_packet_num_of_bytes = (message_.msg_len() - protocol::HEADER_LENGTH) - (num_of_full_packets * MAX_DATAGRAM_SIZE);

    udp_socket.send(boost::asio::buffer(message_.body().get() + MAX_DATAGRAM_SIZE * (num_of_full_packets), last_packet_num_of_bytes) );
}

void Connection::send_body(tcp::socket& tcp_socket) {
    boost::asio::write(tcp_socket, boost::asio::buffer(message_.body().get(), message_.msg_len() - protocol::HEADER_LENGTH));
}

void Connection::send_with_confirmation(tcp::socket& tcp_socket, protocol::message_type expected_confirmation) {
    send_header();

    protocol::message_type confirmation = read_confirmation();
    if(!is_confirmation_correct(expected_confirmation, confirmation)) {
        throw std::runtime_error("Wrong confirmation number");
    }

    send_body(tcp_socket);
}

void Connection::send_with_confirmation(udp::socket& udp_socket, protocol::message_type expected_confirmation) {
    send_header();

    protocol::message_type confirmation = read_confirmation();
    if(!is_confirmation_correct(expected_confirmation, confirmation)) {
        throw std::runtime_error("Wrong confirmation number");
    }

    send_body(udp_socket);
}

void Connection::read_with_confirmation() {
    protocol::Header& header = message_.get_header();

    read_header();
    header.parse(message_.data().get());

    bool is_confirmation_required = header.get_msg_type() == protocol::message_type::GET_MOVIE;
    if(is_confirmation_required) {
        send_confirmation(header.get_msg_type());
        read_body();
    }
}

void Connection::server_close_btn_clicked() {
    if(tcp_socket_.is_open()) {
        tcp_socket_.close();
    }

    if(udp_socket_.is_open()) {
        udp_socket_.close();
    }
}


