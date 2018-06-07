#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <cstring>
#include "Connection.hpp"
#include "../movie/Resolution.hpp"
#include "protocol/Constants.hpp"

Connection::Connection(boost::asio::io_context& io_context, unique_ptr<MovieLayer>& movie_layer)
    : movie_layer_(movie_layer), tcp_socket_(io_context), udp_socket_(io_context),
      streamed_movie_(nullptr) {
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

    emit user_connects(tcp_socket_.remote_endpoint().address().to_string() + " " + std::to_string(tcp_socket_.remote_endpoint().port()));

    read();
}

void Connection::read() {
    try {
        read_with_confirmation();
    } catch (boost::system::system_error& err) {
        if (err.code().value() == boost::system::errc::no_such_file_or_directory) {
            std::cerr << "Client has disconnected: " << err.what() <<  std::endl;
            emit user_disconnects(tcp_socket_.remote_endpoint().address().to_string() + " " + std::to_string(tcp_socket_.remote_endpoint().port()));
            tcp_socket_.close();
            return;
        }
    } catch (std::exception& err) {
        std::cerr << "Error during read_with_confirmation. Terminating connection." << std::endl;
        tcp_socket_.close();
        return;
    }

    auto msg_type = message_.get_header().get_msg_type();
    switch (msg_type) {
    case protocol::GET_MOVIE_LIST:
        handle_get_movie_list();
        break;
    case protocol::GET_MOVIE:
        handle_get_movie();
        break;
    case protocol::GET_FRAME:
        handle_get_frame();
        break;
    case protocol::MOVIE_FINISHED:
        handle_movie_finished();
        break;
    case protocol::DISCONNECT:
        handle_disconnect();
        break;
    default:
        std::cerr << "Unrecognized header" << std::endl;
        break;
    }
}

void Connection::handle_get_movie_list() {
    string movie_list = get_movie_list(*movie_layer_);
    protocol::message_type msg_type = protocol::GIVE_MOVIE_LIST;

    message_.get_header().set_msg_type(msg_type);
    message_.get_header().set_body_len(movie_list.size());
    message_.set_header(message_.get_header().encode());
    message_.set_body(movie_list);

    try {
        send_with_confirmation(msg_type);
    } catch (std::exception& err) {
        std::cerr << "Error during send_with_confirmation in handle_get_movie_list: " << err.what() << std::endl;
    }

    read();
}

void Connection::handle_get_movie() {
    std::string movie_name = std::string(message_.body().get(), message_.get_header().get_body_len());
    streamed_movie_ = make_unique<Movie>(movie_layer_->get_movie_location(movie_name));

    Frame frame;
    unsigned int numOfFrames;
    protocol::message_type msg_type = protocol::GIVE_FRAME;
    try {
       frame = streamed_movie_->videoStream()
                                .getFrame(0)
                                .resize(Resolution::get144p());

       numOfFrames = streamed_movie_->videoStream().get_num_of_frames();
    } catch (std::out_of_range& err) {
        std::cerr << "No such movie on server. Exiting" << std::endl;
        return;
    }

    message_.get_header().set_msg_type(msg_type);
    message_.get_header().set_body_len(frame.data_length());
    message_.get_header().set_num_of_cols(frame.getSize().width);
    message_.get_header().set_num_of_rows(frame.getSize().height);
    message_.get_header().set_frame_num(0);
    message_.get_header().set_num_of_frames(numOfFrames);
    message_.set_header(message_.get_header().encode());
    message_.set_body(frame.data());

    try {
        send_with_confirmation(msg_type);
    } catch (std::exception& err) {
        std::cerr << "Error during send_with_confirmation in handle_get_movie: " << err.what() << std::endl;
    }

    read();
}

void Connection::handle_get_frame() {
    Frame frame;
    unsigned num_of_frames;
    protocol::Header& header = message_.get_header();
    protocol::message_type msg_type = protocol::message_type::GIVE_FRAME;

    try {
        frame = streamed_movie_->videoStream()
                                .getFrame(message_.get_header().get_frame_num())
                                .resize(Resolution::get144p());

        num_of_frames = streamed_movie_->videoStream().get_num_of_frames();
    } catch (std::exception& err) {
        std::cerr << "An error occured during frame data retrive in handle_get_frame:" << err.what() << std::endl;
        std::cerr << "Disconnecting user" << std::endl;
        handle_disconnect();
        return;
    }

    /*
     * No need to set frame number
     * Frame number: provided by user in request
     */
    message_.get_header().set_msg_type(msg_type);
    message_.get_header().set_body_len(frame.data_length());
    message_.get_header().set_num_of_cols(frame.getSize().width);
    message_.get_header().set_num_of_rows(frame.getSize().height);
    // TODO: If client will always send num of frames then server don't need to set this field
    message_.get_header().set_num_of_frames(num_of_frames);
    message_.set_header(message_.get_header().encode());
    message_.set_body(frame.data());

    try {
        send_with_confirmation(msg_type);
    } catch (std::exception& err) {
        std::cerr << "Error during send_with_confirmation in handle_get_frame: " << err.what() << std::endl;
    }

    read();
}

void Connection::handle_disconnect() {
    emit user_disconnects(tcp_socket_.remote_endpoint().address().to_string() + " " + std::to_string(tcp_socket_.remote_endpoint().port()));

    tcp_socket_.close();
    udp_socket_.close();
}

void Connection::handle_movie_finished() {
    read();
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
    std::string header(message_.data().get(), 34);
    std::cerr << "Header to send: " << header << std::endl;

    boost::asio::write(tcp_socket_, boost::asio::buffer(message_.data().get(), protocol::HEADER_LENGTH));
}

void Connection::send_body() {
    boost::asio::write(tcp_socket_, boost::asio::buffer(message_.body().get(), message_.msg_len() - protocol::HEADER_LENGTH));
}

void Connection::send_with_confirmation(protocol::message_type expected_confirmation) {
    send_header();

    protocol::message_type confirmation = read_confirmation();
    if(!is_confirmation_correct(expected_confirmation, confirmation)) {
        throw std::runtime_error("Wrong confirmation number");
    }

    send_body();
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


