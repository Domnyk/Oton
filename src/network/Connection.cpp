#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <thread>
#include <cstring>
#include "Connection.hpp"
#include "../movie/Resolution.hpp"
#include "protocol/Constants.hpp"

Connection::Connection(boost::asio::io_context& io_context, unique_ptr<MovieList>& movie_list)
    : movie_list_(movie_list), tcp_socket_(io_context), udp_socket_(io_context),
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
    unsigned const short port_num_len = 5;
    boost::array<char, port_num_len> port_num_buf;
    try {
        boost::asio::read(tcp_socket_, boost::asio::buffer(port_num_buf, port_num_buf.size()));
    } catch (boost::system::system_error& err) {
        std::cerr << "Error during Connection::start() when reading from tcp_socket: " << err.what() << ". Terminating" << std::endl;
        return;
    }

    // Parse port num and set it
    unsigned short client_udp_port = std::stoi(std::string(port_num_buf.data(), port_num_buf.size()));
    std::cerr << "UDP Port: " << client_udp_port << std::endl;
    try {
        udp_socket_.connect(udp::endpoint(tcp_socket_.remote_endpoint().address(), client_udp_port));
    } catch (std::exception& err) {
        std::cerr << "Error in udp_socket connect" << std::endl;
    }

    // Send server udp port num
    unsigned short server_udp_port = udp_socket_.local_endpoint().port();
    std::string server_port_num_str = std::to_string(server_udp_port);
    try {
        boost::asio::write(tcp_socket_, boost::asio::buffer(server_port_num_str, server_port_num_str.size()));
    } catch (boost::system::system_error& err) {
        std::cerr << "Error during Connection::start() when writing to tcp_socket: " << err.what() << ". Terminating" << std::endl;
        return;
    }

    id_string_ = tcp_socket_.remote_endpoint().address().to_string() + " " + std::to_string(tcp_socket_.remote_endpoint().port());
    emit user_connects(id_string_);

    communicate();
}

void Connection::communicate() {
    bool is_client_ok = true;

    while(is_client_ok) {
        try {
            read_with_confirmation();
        } catch (boost::system::system_error& err) {
            if (err.code().value() == boost::system::errc::no_such_file_or_directory) {
                std::cerr << "Client has disconnected" << std::string(err.what()) << std::endl;
                emit user_disconnects(id_string_);
                return;
            }
        } catch (std::exception& err) {
            std::cerr << "Error during read_with_confirmation. Terminating connection." << std::endl;
            return;
        }

        std::cerr << "Received header is: " << message_.get_header().encode() << std::endl;

        auto msg_type = message_.get_header().get_msg_type();
        is_client_ok = handle_received_msg(msg_type);
    }

    disconnect_client();
}

bool Connection::handle_received_msg(protocol::message_type msg_type) {
    bool is_client_ok = true;

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
        std::cerr << "Unrecognized header" << std::endl;
        break;
    }

    return is_client_ok;
}

bool Connection::handle_get_movie_list() {
    bool is_client_ok = true;
    string movie_list = encode_movie_list_as_string(*movie_list_);
    protocol::message_type msg_type = protocol::GIVE_MOVIE_LIST;

    message_.get_header().set_msg_type(msg_type);
    message_.get_header().set_body_len(movie_list.size());
    message_.set_header(message_.get_header().encode());
    message_.set_body(movie_list);

    try {
        send_with_confirmation(tcp_socket_, msg_type);
    } catch (std::exception& err) {
        std::cerr << "Error during send_with_confirmation in handle_get_movie_list: " << err.what() << std::endl;
        is_client_ok = false;
    }

    return is_client_ok;
}

void Connection::send_frame(unsigned int frame_num) {
    protocol::message_type msg_type = protocol::GIVE_FRAME;
    Frame frame = streamed_movie_->videoStream()
                              .get_frame(frame_num)
                              .resize(Resolution::get144p());

    unsigned int num_of_frames = streamed_movie_->videoStream().get_num_of_frames();

    prepare_header(msg_type, frame.data_length(), frame.get_size().width,
                    frame.get_size().height, num_of_frames, frame_num, frame.is_key_frame());
    message_.set_header(message_.get_header().encode());
    message_.set_body(frame.data());

    send_msg_with_frame(frame, msg_type);
}

bool Connection::handle_get_movie() {
    std::string movie_name = std::string(message_.body().get(), message_.get_header().get_body_len());

    try {
        streamed_movie_ = make_unique<Movie>(movie_list_->get_movie_location(movie_name));
    } catch (std::exception& err) {
        std::cerr << "Error in Connection::handle_get_movie during get_movie_location. Client requested nonexisting movie";
        return false;
    }

    try {
        send_frame(0);
    } catch (std::exception& err) {
        std::cerr << "Error during send_frame(0): " << err.what() << std::endl;
    }

    return true;
}

bool Connection::handle_get_frame() {
    if(!streamed_movie_) {
        std::cerr << "No movie location set for server. Terminating connection" << std::endl;
        return false;
    }

    try {
        send_frame(message_.get_header().get_frame_num());
    } catch (std::exception& err) {
        std::cerr << "Error during send_frame() in Connection::handle_get_frame(): " << err.what() << std::endl;
        return false;
    }

    return true;
}

void Connection::send_msg_with_frame(const Frame& frame, protocol::message_type msg_type) {
    if (frame.is_key_frame()) {
            std::cerr << "Send by TCP" << std::endl;
            send_with_confirmation(tcp_socket_, msg_type);
    } else {
            std::cerr << "Send by UDP" << std::endl;
            send_with_confirmation(udp_socket_, msg_type);
    }
}

bool Connection::disconnect_client() {
    emit user_disconnects(id_string_);

    if(tcp_socket_.is_open()) {
        try {
            tcp_socket_.close();
        } catch (std::exception& err) {
            std::cerr << "Error during TCP socket close: " << err.what() << std::endl;
        }
    }

    if(udp_socket_.is_open()) {
        try {
            udp_socket_.close();
        } catch (std::exception& err) {
            std::cerr << "Error during UDP socket close: " << err.what() << std::endl;
        }
    }



    return true;
}

bool Connection::handle_movie_finished() {
    return true;
}

protocol::message_type Connection::read_confirmation() {
    char confirmation[protocol::FieldLength::msg_type + 1] = "";

    try {
        boost::asio::read(tcp_socket_, boost::asio::buffer(confirmation, protocol::FieldLength::msg_type));
    } catch (boost::system::system_error& err) {
        std::cerr << "An error occured during confirmation reading: " << err.what() << std::endl;
    }

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
    std::cerr << "Header to send: " << header << std::endl;

    boost::asio::write(tcp_socket_, boost::asio::buffer(message_.data().get(), protocol::HEADER_LENGTH));
}

void Connection::send_body(udp::socket& udp_socket) {
    unsigned num_of_full_packets = (message_.msg_len() - protocol::HEADER_LENGTH) / protocol::MAX_DATAGRAM_SIZE;

    for (unsigned short i = 0; i < num_of_full_packets; ++i) {
        udp_socket.send(boost::asio::buffer(message_.body().get() + protocol::MAX_DATAGRAM_SIZE * i, protocol::MAX_DATAGRAM_SIZE));
    }

    unsigned last_packet_num_of_bytes = (message_.msg_len() - protocol::HEADER_LENGTH) - (num_of_full_packets * protocol::MAX_DATAGRAM_SIZE);

    udp_socket.send(boost::asio::buffer(message_.body().get() + protocol::MAX_DATAGRAM_SIZE * (num_of_full_packets), last_packet_num_of_bytes) );
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

void Connection::prepare_header(protocol::message_type msg_type, unsigned body_len, unsigned num_of_cols,
                                unsigned num_of_rows, unsigned num_of_frames, unsigned frame_num, bool is_key_frame) {
    protocol::Header& header = message_.get_header();

    header.set_msg_type(msg_type);
    header.set_body_len(body_len);
    header.set_num_of_cols(num_of_cols);
    header.set_num_of_rows(num_of_rows);
    header.set_num_of_frames(num_of_frames);
    header.set_frame_num(frame_num);
    header.set_is_key_frame(is_key_frame);
}


