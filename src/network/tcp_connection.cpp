#include <iostream>
#include <cstring>
#include <chrono>
#include <boost/array.hpp>
#include "tcp_connection.hpp"
#include "../movie/Resolution.hpp"
#include "protocol/Constants.hpp"

const unsigned short tcp_connection::SPEED_CONTROL_PERIOD = 20;

tcp_connection::tcp_connection(boost::asio::io_context& io_context, std::unique_ptr<movie_layer>& movie_layer):
    message_(), movie_layer_(movie_layer), socket_(io_context), frames_since_speed_control_(0),
    client_download_speed_(0) {

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
    } catch (boost::system::system_error& err) {
        if (err.code().value() == boost::system::errc::no_such_file_or_directory) {
            std::cerr << "Client has disconnected" << std::endl;
            return;
        }
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
    case protocol::SPEED_CONTROL_INIT:
        handle_speed_control_init();
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
        std::cerr << "Error during send_with_confirmation in handle_get_movie: " << err.what() << ". Frame counter not increased." << std::endl;
        return;
    }

    ++frames_since_speed_control_;
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
        std::cerr << "Error during send_with_confirmation in handle_get_frame: " << err.what() << "Frame counter not increased." << std::endl;
        return;
    }

    ++frames_since_speed_control_;
}

void tcp_connection::handle_disconnect() {
    socket_.close();
}

void tcp_connection::handle_speed_control_init() {
    using namespace std::chrono;

    unsigned long long ts_1, ts_2;
    const unsigned msg_size = 1000;
    const unsigned short timestamp_length = 20;
    const unsigned short msg_type_index = 1;
    const unsigned short speed_control_msg_size = 54;
    boost::array<char, msg_size> buf;
    buf.fill('0');

    // Send dumb packet
    buf.at(msg_type_index) = static_cast<char>(message_.get_header().get_msg_type());
    ts_1 = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
    try {
        boost::asio::write(socket_, boost::asio::buffer(buf, msg_size));
    } catch (std::exception& err) {
        std::cerr << "Error during sending of speed control message: " << err.what() << std::endl;
    }

    // Read response
    try {
        boost::asio::read(socket_, boost::asio::buffer(message_.data(), speed_control_msg_size));
    } catch (std::exception& err) {
        std::cerr << "Error during reading of speed control message: " << err.what() << std::endl;
    }

    // Process response
    message_.get_header().parse();
    if (!(protocol::message_type::SPEED_CONTROL_FIN == message_.get_header().get_msg_type())) {
        throw std::runtime_error("Speed control: expected SPEED_CONTROL_FIN");
    }

    ts_2 = std::stoll(std::string(message_.body(), timestamp_length));

    try {
        send_confirmation(protocol::message_type::SPEED_CONTROL_FIN);
    } catch (std::exception& err) {
        std::cerr << "Error during sending of speed control end: " << err.what() << std::endl;
    }

    std::cerr << "Value set by handle_speed_control_init is untrusted. Check it out" << std::endl;
    std::cerr << "Time window is: " << ts_2 - ts_1 << std::endl;
    std::cerr << "This value is: " << msg_size / ((ts_2 - ts_1) / 1000) << std::endl;
}

void tcp_connection::handle_movie_finished() {
    // Nothing for now
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

    bool is_empty_body = header.get_body_len() == 0;
    if(is_empty_body) {
        std::cerr << "Empty body. No need to call read_body()" << std::endl;
    }

    read_body();
}


