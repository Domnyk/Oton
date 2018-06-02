#include <boost/bind.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <cstring>
#include "tcp_connection.hpp"
#include "../movie/Resolution.hpp"
#include "protocol/Constants.hpp"

tcp_connection::tcp_connection(boost::asio::io_context& io_context, std::unique_ptr<movie_layer>& movie_layer):
    message_(), movie_layer_(movie_layer), socket_(io_context), in_packet_() {

}

tcp_connection::shared_pointer tcp_connection::create(boost::asio::io_context& io_context, std::unique_ptr<movie_layer>& movie_layer) {
    return tcp_connection::shared_pointer(new tcp_connection(io_context, movie_layer));
}

tcp::socket& tcp_connection::socket() {
	return socket_;
}

void tcp_connection::start_read() {

    try {
        read_header();
    } catch (boost::system::system_error& ec) {
        std::cerr << "An error occured during header reading: " << ec.what()  << ". Exiting." << std::endl;
        return;
    }
    message_.get_header().parse();

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

    start_read();
}

void tcp_connection::handle_get_movie_list() {
    string movie_list = movie_layer_->get_movie_list();
    protocol::message_type msg_type = protocol::GIVE_MOVIE_LIST;

    message_.get_header().set_msg_type(msg_type);
    message_.get_header().set_body_len(movie_list.size());
    message_.get_header().encode();
    try {
        send_header();
    } catch (boost::system::system_error& err) {
        std::cerr << "An error occured during socket write: " << err.what() << std::endl;
    }

    protocol::message_type confirmation = read_confirmation();
    if (!is_confirmation_correct(msg_type, confirmation)) {
        std::cerr << "Bad confirmation. Exiting" << std::endl;
        return;
    }

    message_.set_body(movie_list);
    try {
        boost::asio::write(socket_, boost::asio::buffer(message_.body(), message_.msg_len() - protocol::Message::HEADER_LENGTH));
    } catch (boost::system::system_error& err) {
        std::cerr << "An error occured during socket write: " << err.what() << std::endl;
    }
}

void tcp_connection::handle_get_movie() {
    send_confirmation(protocol::message_type::GET_MOVIE);
    read_body();

    std::string movie_name = std::string(message_.body(), message_.get_header().get_body_len());
    std::cerr << "Movie name is: " << movie_name << std::endl;

    Frame frame;
    unsigned int numOfFrames;
    try {
       frame = movie_layer_->get_movie(movie_name)
                             .videoStream()
                             .getFrame(100)
                             .resize(Resolution::get360p());

       numOfFrames = movie_layer_->get_movie(movie_name)
                                   .videoStream().get_num_of_frames();
    } catch (std::out_of_range& err) {
        std::cerr << "No such movie on server. Exiting" << std::endl;
        return;
    }

    message_.get_header().set_msg_type(protocol::GIVE_FRAME);
    message_.get_header().set_body_len(frame.data_length());
    message_.get_header().set_num_of_cols(frame.getSize().width);
    message_.get_header().set_num_of_rows(frame.getSize().height);
    message_.get_header().set_frame_num(0);
    message_.get_header().set_num_of_frames(numOfFrames);
    message_.get_header().encode();
    send_header();

    protocol::message_type confirmation = read_confirmation();
    if(!is_confirmation_correct(protocol::GIVE_FRAME, confirmation)) {
        std::cerr << "Wrong confirmation number. Exiting" << std::endl;
        return;
    }

    message_.set_body(frame.data());
    try {
        send_body();
    } catch (boost::system::system_error& err) {
        std::cerr << "An error occured during frame data writing: " << err.what() << std::endl;
    }
}

void tcp_connection::handle_get_frame() {
    Frame frame = movie_layer_->get_movie("Megamind.avi")
                                .videoStream()
                                .getFrame(message_.get_header().get_frame_num())
                                .resize(Resolution::get144p());

    message_.get_header().set_msg_type(protocol::GIVE_FRAME);
    message_.get_header().set_body_len(frame.data_length());
    message_.get_header().set_num_of_cols(frame.getSize().width);
    message_.get_header().set_num_of_rows(frame.getSize().height);
    message_.get_header().encode();
    // Frame number is already set
    // Ignore num of frames for now
    std::cerr << "Add num of frames header" << std::endl;
    std::cerr << "Body len: " << message_.get_header().get_body_len() << std::endl;

    send_header();




    // message_.set_body((char*)frame.data());

    /* std::cerr << "Data length: " << frame.data_length() << std::endl;
    std::cerr << "Rows: " << frame.getPixelMatrix().rows << std::endl;
    std::cerr << "Cols: " << frame.getPixelMatrix().cols << std::endl; */

    /* auto me = shared_from_this();
    boost::asio::async_write(socket_,
                             boost::asio::buffer(frame.data(), frame.data_length()),
                             [=](auto& ec, unsigned int bytes_transferred) {
                                if(ec) {
                                    std::cerr << "Error occured during sending" << std::endl;
                                    return;
                                }

                                me->start_read();
                             }
    ); */
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

    try {
        boost::asio::write(socket_, boost::asio::buffer(confirmation.c_str(), protocol::FieldLength::msg_type));
    } catch (boost::system::system_error& err) {
        std::cerr << "An error occured during confirmation sending: " << err.what() << std::endl;
    }
}

bool tcp_connection::is_confirmation_correct(protocol::message_type msg_type, protocol::message_type confirmation) {
    return msg_type == confirmation;
}

void tcp_connection::read_header() {
    boost::asio::read(socket_, boost::asio::buffer(message_.data(), protocol::Message::HEADER_LENGTH));
}

void tcp_connection::read_body() {
    try {
        boost::asio::read(socket_, boost::asio::buffer(message_.body(), message_.get_header().get_body_len()));
    } catch (boost::system::system_error& err) {
        std::cerr << "An error occured during body reading: " << err.what() << std::endl;
        return;
    }
}


void tcp_connection::send_header() {
    boost::asio::write(socket_, boost::asio::buffer(message_.data(), protocol::Message::HEADER_LENGTH));
}

void tcp_connection::send_body() {
    boost::asio::write(socket_, boost::asio::buffer(message_.body(), message_.msg_len() - protocol::Message::HEADER_LENGTH));
}

/*void tcp_connection::handle_read_body_get_frame() {
    auto me = shared_from_this();

    std::istream stream(&parser_.get_streambuf());
    std::string body_string;
    stream >> body_string;

    int frame_num;
    Frame frame;
    try {
        frame_num = std::stoi(body_string);
        frame = movie_layer_->get_movie("Megamind.avi").videoStream().getFrame(frame_num).resize(Resolution::get360p());
        std::cerr << "Data length: " << frame.data_length() << std::endl;
        std::cerr << "Rows: " << frame.getPixelMatrix().rows << std::endl;
        std::cerr << "Cols: " << frame.getPixelMatrix().cols << std::endl;


    } catch (std::invalid_argument& e) {
        start_read_header();
    }

     socket_.async_send(boost::asio::buffer(frame.data(), frame.data_length()),
                       [me](auto& ec, auto bytes_transferred) {
                        if(ec) {
                            std::cerr << "Error occured during sending" << std::endl;
                            return;
                        }
                            std::cerr << "Transferred: " << bytes_transferred << std::endl;
                            me->start_read();
                        }
    );

     boost::asio::async_write(socket_, boost::asio::buffer(frame.data(), frame.data_length()),
                            [me](auto& ec, auto bytes_transferred) {
                            if(ec) {
                                std::cerr << "Error occured during sending" << std::endl;
                                return;
                             }
                                std::cerr << "Transferred: " << bytes_transferred << std::endl;
                                me->start_read();
                            }
    )

    // start_read();
} */


