#include <boost/bind.hpp>
#include <iostream>

#include "tcp_connection.hpp"
#include "tcp_server.hpp"

const int ANY_PORT = 0;

tcp_server::tcp_server(boost::asio::io_context& io_context)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), ANY_PORT)) {
    start_accept();
}

void tcp_server::start_accept() {
	tcp_connection::shared_pointer new_connection = tcp_connection::create(acceptor_.get_executor().context());

	acceptor_.async_accept(
		new_connection->socket(),
		boost::bind(&tcp_server::handle_accept, this, new_connection, boost::asio::placeholders::error)
	) ;
}

void tcp_server::handle_accept(tcp_connection::shared_pointer new_conection, const boost::system::error_code& error) {
	if (!error) {
		new_conection->start();
	}

	start_accept();
}

