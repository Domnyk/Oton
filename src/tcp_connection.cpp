#include <boost/bind.hpp>

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

void tcp_connection::start() {
	message_ = make_daytime_string();

	boost::asio::async_write(
		socket_,
		boost::asio::buffer(message_),
		boost::bind(
			&tcp_connection::handle_write,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
			)
	);
}