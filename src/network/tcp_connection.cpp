#include <boost/bind.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <cstring>
#include "tcp_connection.hpp"
#include "../movie/Resolution.hpp"
#include "protocol/Constants.hpp"

tcp_connection::tcp_connection(boost::asio::io_context& io_context, std::unique_ptr<movie_layer>& movie_layer):
    message_(), movie_layer_(movie_layer), socket_(io_context) {

}

tcp_connection::shared_pointer tcp_connection::create(boost::asio::io_context& io_context, std::unique_ptr<movie_layer>& movie_layer) {
    return tcp_connection::shared_pointer(new tcp_connection(io_context, movie_layer));
}

tcp::socket& tcp_connection::get_socket() {
	return socket_;
}

void tcp_connection::read() {
    try {
        read_with_confirmation();
    } catch (std::exception& err) {
        std::cerr << "Error during read_with_confirmation. Terminating connection." << std::endl;
        socket_.close();
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
        break;
    case protocol::DISCONNECT:
        break;
    default:
        break;
    }

    read();
}

void tcp_connection::handle_get_movie_list() {
    string movie_list = movie_layer_->get_movie_list();
    protocol::message_type msg_type = protocol::GIVE_MOVIE_LIST;

    message_.get_header().set_msg_type(msg_type);
    message_.get_header().set_body_len(movie_list.size());
    message_.get_header().encode();
    message_.set_body(movie_list);

    try {
        send_with_confirmation(msg_type);
    } catch (std::exception& err) {
        std::cerr << "Error during send_with_confirmation in handle_get_movie_list: " << err.what() << std::endl;
    }
}

void tcp_connection::handle_get_movie() {
    std::string movie_name = std::string(message_.body(), message_.get_header().get_body_len());
    streamed_movie_ = movie_name;

    std::cerr << "Requested movie is: " << streamed_movie_ << std::endl;

    Frame frame;
    unsigned int numOfFrames;
    protocol::message_type msg_type = protocol::GIVE_FRAME;
    try {
       frame = movie_layer_->get_movie(streamed_movie_)
                             .videoStream()
                             .getFrame(0)
                             .resize(Resolution::get144p());

       numOfFrames = movie_layer_->get_movie(movie_name)
                                   .videoStream().get_num_of_frames();
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
    message_.get_header().encode();
    message_.set_body(frame.data());

    try {
        send_with_confirmation(msg_type);
    } catch (std::exception& err) {
        std::cerr << "Error during send_with_confirmation in handle_get_movie: " << err.what() << std::endl;
    }
}

void tcp_connection::handle_get_frame() {
    Frame frame;
    unsigned num_of_frames;
    protocol::Header& header = message_.get_header();
    protocol::message_type msg_type = protocol::message_type::GIVE_FRAME;

    try {
        VideoStream& vs = movie_layer_->get_movie(streamed_movie_).videoStream();

        frame = vs.getFrame(message_.get_header().get_frame_num())
                        .resize(Resolution::get144p());

        num_of_frames = vs.get_num_of_frames();
    } catch (std::exception& err) {
        std::cerr << "An error occured during frame data retrive in handle_get_frame:" << err.what() << std::endl;
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
    message_.get_header().encode();
    message_.set_body(frame.data());

    try {
        send_with_confirmation(msg_type);
    } catch (std::exception& err) {
        std::cerr << "Error during send_with_confirmation in handle_get_frame: " << err.what() << std::endl;
    }
}

protocol::message_type tcp_connection::read_confirmation() {
    char confirmation[protocol::FieldLength::msg_type + 1] = "";

    try {
        boost::asio::read(socket_, boost::asio::buffer(confirmation, protocol::FieldLength::msg_type));
    } catch (boost::system::system_error& err) {
        std::cerr << "An error occured during confirmation reading: " << err.what() << std::endl;
    }

    auto confirmation_num = std::stoi(std::string(confirmation));
    return static_cast<protocol::message_type>(confirmation_num);
}

void tcp_connection::send_confirmation(protocol::message_type msg_type) {
    std::string confirmation = "0" + to_string(msg_type);
    boost::asio::write(socket_, boost::asio::buffer(confirmation.c_str(), protocol::FieldLength::msg_type));
}

bool tcp_connection::is_confirmation_correct(protocol::message_type msg_type, protocol::message_type confirmation) {
    return msg_type == confirmation;
}

void tcp_connection::read_header() {
    boost::asio::read(socket_, boost::asio::buffer(message_.data(), protocol::Message::HEADER_LENGTH));
}

void tcp_connection::read_body() {
    boost::asio::read(socket_, boost::asio::buffer(message_.body(), message_.get_header().get_body_len()));
}

void tcp_connection::send_header() {
    std::string header(message_.data(), 34);
    std::cerr << "Header to send: " << header << std::endl;

    boost::asio::write(socket_, boost::asio::buffer(message_.data(), protocol::Message::HEADER_LENGTH));
}

void tcp_connection::send_body() {
    boost::asio::write(socket_, boost::asio::buffer(message_.body(), message_.msg_len() - protocol::Message::HEADER_LENGTH));
}

void tcp_connection::send_with_confirmation(protocol::message_type expected_confirmation) {
    send_header();

    protocol::message_type confirmation = read_confirmation();
    if(!is_confirmation_correct(expected_confirmation, confirmation)) {
        throw std::runtime_error("Wrong confirmation number");
    }

    send_body();
}

void tcp_connection::read_with_confirmation() {
    protocol::Header& header = message_.get_header();

    read_header();
    header.parse();

    bool is_confirmation_required = header.get_msg_type() == protocol::message_type::GET_MOVIE;
    if(is_confirmation_required) {
        send_confirmation(header.get_msg_type());
    }

    read_body();
}


