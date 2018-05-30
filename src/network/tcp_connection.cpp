#include <boost/bind.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include "tcp_connection.hpp"
#include "../movie/Resolution.hpp"


tcp_connection::tcp_connection(boost::asio::io_context& io_context, std::unique_ptr<movie_layer>& movie_layer):
    socket_(io_context), parser_(), movie_layer_(movie_layer) {

}

tcp_connection::shared_pointer tcp_connection::create(boost::asio::io_context& io_context, std::unique_ptr<movie_layer>& movie_layer) {
    return tcp_connection::shared_pointer(new tcp_connection(io_context, movie_layer));
}

tcp::socket& tcp_connection::socket() {
	return socket_;
}

void tcp_connection::handle_write(const boost::system::error_code&, size_t) {
}

void tcp_connection::start_read() {
    std::cout << "Executed TCP start_read" << std::endl;

    auto me = shared_from_this();

    boost::asio::async_read(socket_,
                            boost::asio::buffer(parser_.data(), parser_.header_length()),
                             [=] (auto ec, std::size_t) {
                                me->handle_read(ec);
                             }
    );
}

void tcp_connection::handle_read(const boost::system::error_code& ec) {
    if (ec) {
        std::cerr << "Error over TCP async_read" << std::endl;
        return;
    }

    /* protocol::Header header;
    try {
        header = parser_.parse_header();
    } catch (std::invalid_argument& err) {
        std::cerr << "Received invalid header over TCP" << std::endl;
        start_read();
        return;
    } */

    /* switch(header.type()) {
        case protocol::GET_FRAME:
            // Get frame data
            std::cout << "Received header GET FRAME" << std::endl;
            start_read_body_get_frame();
            // Send frame data
        break;
        default:
            std::cerr << "Unkown header type for TCP" << std::endl;
            start_read();
    } */

    start_read();
}

void tcp_connection::start_read_body_get_frame() {
    std::cout << "TCP: start_read_body_get_frame" << std::endl;

    auto me = shared_from_this();

    boost::asio::async_read_until(socket_,
                                  parser_.get_streambuf(),
                                  '\n',
                                  [me](const boost::system::error_code& e, std::size_t size) {
                                    me->handle_read_body_get_frame();
                                  }
    );
}

void tcp_connection::handle_read_body_get_frame() {
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
        start_read();
    }

    /* socket_.async_send(boost::asio::buffer(frame.data(), frame.data_length()),
                       [me](auto& ec, auto bytes_transferred) {
                        if(ec) {
                            std::cout << "Error occured during sending" << std::endl;
                            return;
                        }
                            std::cout << "Transferred: " << bytes_transferred << std::endl;
                            me->start_read();
                        }
    ); */

    boost::asio::async_write(socket_, boost::asio::buffer(frame.data(), frame.data_length()),
                            [me](auto& ec, auto bytes_transferred) {
                            if(ec) {
                                std::cout << "Error occured during sending" << std::endl;
                                return;
                             }
                                std::cout << "Transferred: " << bytes_transferred << std::endl;
                                me->start_read();
                            }
    );

    // start_read();
}


