#include <boost/bind.hpp>
#include <iostream>

#include "tcp_connection.hpp"

std::string make_daytime_string()
{
  using namespace std;
  time_t now = time(0);
  return ctime(&now);
}

tcp_connection::shared_pointer tcp_connection::create(boost::asio::io_context& io_context) {
	return tcp_connection::shared_pointer(new tcp_connection(io_context));
}

tcp::socket& tcp_connection::socket() {
	return socket_;
}

tcp_connection::tcp_connection(boost::asio::io_context& io_context) : socket_(io_context) {
}

void tcp_connection::handle_write(const boost::system::error_code&, size_t) {
}

void tcp_connection::start_send(unsigned char* data, unsigned int data_length) {
    auto me = shared_from_this();

    std::cout << "data_length: " << data_length << std::endl;

    boost::asio::async_write(socket_,
                             boost::asio::buffer(data, data_length),
                             [=] (auto ec, std::size_t) {
                                me->handle_send(ec);
                             }
    );
}

void tcp_connection::handle_send(const boost::system::error_code& ec) {

}


