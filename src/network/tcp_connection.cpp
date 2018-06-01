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
        boost::asio::read(socket_, boost::asio::buffer(message_.data(), protocol::Message::HEADER_LENGTH));
    } catch (boost::system::system_error& err) {
        std::cerr << "An error occured during socket read: " << err.what() << std::endl;
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

void tcp_connection::handle_read() {
    /* std::istream stream(&in_packet_);
    std::string packet_string;
    stream >> packet_string;
    std::cout << "Packet string: " << packet_string << std::endl;

    size_t end_tag_pos = packet_string.find(protocol::Message::END_TAG);
    if (end_tag_pos == string::npos) {
        throw std::runtime_error("No delimiter in received message");
    }

    packet_string.erase(end_tag_pos);

    message_.set_data(packet_string);
    message_.get_header().parse(); */
}

void tcp_connection::handle_get_movie_list() {
    string movie_list = movie_layer_->get_movie_list();

    message_.get_header().set_msg_type(protocol::GIVE_MOVIE_LIST);
    message_.get_header().set_body_len(movie_list.size());
    message_.get_header().encode();
    try {
        boost::asio::write(socket_, boost::asio::buffer(message_.data(), protocol::Message::HEADER_LENGTH));
    } catch (boost::system::system_error& err) {
        std::cerr << "An error occured during socket write: " << err.what() << std::endl;
    }

    protocol::message_type confirmation_num;
    try {
        confirmation_num = process_confirmation();
    } catch (std::invalid_argument& err) {
        std::cerr << "Could not convert confirmation number. Exiting" << std::endl;
        return;
    }

    if(!(confirmation_num == protocol::GIVE_MOVIE_LIST)) {
        std::cerr << "Wrong confirmation number. Exiting" << std::endl;
        return;
    }

    message_.set_body(movie_list);
    try {
        boost::asio::write(socket_, boost::asio::buffer(message_.body(), message_.msg_len() - protocol::Message::HEADER_LENGTH));
    } catch (boost::system::system_error& err) {
        std::cerr << "An error occured during socket write: " << err.what() << std::endl;
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
    // Frame number is already set
    // Ignore num of frames for now
    std::cerr << "Add num of frames header" << std::endl;

    message_.set_body((char*)frame.data());

    /* std::cout << "Data length: " << frame.data_length() << std::endl;
    std::cout << "Rows: " << frame.getPixelMatrix().rows << std::endl;
    std::cout << "Cols: " << frame.getPixelMatrix().cols << std::endl; */

    auto me = shared_from_this();
    boost::asio::async_write(socket_,
                             boost::asio::buffer(frame.data(), frame.data_length()),
                             [=](auto& ec, unsigned int /* bytes_transferred */) {
                                if(ec) {
                                    std::cout << "Error occured during sending" << std::endl;
                                    return;
                                }

                                me->start_read();
                             }
    );
}

void tcp_connection::handle_get_movie() {
    Frame frame = movie_layer_->get_movie("Megamind.avi")
                                .videoStream()
                                .getFrame(0)
                                .resize(Resolution::get144p());

    message_.get_header().set_msg_type(protocol::GIVE_FRAME);
    message_.get_header().set_body_len(frame.data_length());
    message_.get_header().set_num_of_cols(frame.getSize().width);
    message_.get_header().set_num_of_rows(frame.getSize().height);

    message_.set_body((char*)frame.data());

    auto me = shared_from_this();
    boost::asio::async_write(socket_,
                             boost::asio::buffer(frame.data(), frame.data_length()),
                             [=](auto& ec, unsigned int /* bytes_transferred */) {
                                if(ec) {
                                    std::cout << "Error occured during sending" << std::endl;
                                    return;
                                }

                                me->start_read();
                             }
    );
}

protocol::message_type tcp_connection::process_confirmation() {
    char confirmation[protocol::FieldLength::msg_type + 1] = "";

    try {
        boost::asio::read(socket_, boost::asio::buffer(confirmation, 2));
    } catch (boost::system::system_error& err) {
        std::cerr << "An error occured during confirmation read: " << err.what() << std::endl;
    }

    auto confirmation_num = std::stoi(std::string(confirmation));
    return static_cast<protocol::message_type>(confirmation_num);
}

/* void tcp_connection::start_read_body_get_frame() {
    std::cout << "TCP: start_read_body_get_frame" << std::endl;

    auto me = shared_from_this();

     boost::asio::async_read_until(socket_,
                                  parser_.get_streambuf(),
                                  '\n',
                                  [me](const boost::system::error_code& e, std::size_t size) {
                                    me->handle_read_body_get_frame();
                                  }
    );
} */

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
        std::cout << "Data length: " << frame.data_length() << std::endl;
        std::cout << "Rows: " << frame.getPixelMatrix().rows << std::endl;
        std::cout << "Cols: " << frame.getPixelMatrix().cols << std::endl;


    } catch (std::invalid_argument& e) {
        start_read_header();
    }

     socket_.async_send(boost::asio::buffer(frame.data(), frame.data_length()),
                       [me](auto& ec, auto bytes_transferred) {
                        if(ec) {
                            std::cout << "Error occured during sending" << std::endl;
                            return;
                        }
                            std::cout << "Transferred: " << bytes_transferred << std::endl;
                            me->start_read();
                        }
    );

     boost::asio::async_write(socket_, boost::asio::buffer(frame.data(), frame.data_length()),
                            [me](auto& ec, auto bytes_transferred) {
                            if(ec) {
                                std::cout << "Error occured during sending" << std::endl;
                                return;
                             }
                                std::cout << "Transferred: " << bytes_transferred << std::endl;
                                me->start_read();
                            }
    )

    // start_read();
} */


