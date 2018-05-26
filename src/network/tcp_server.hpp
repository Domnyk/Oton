#pragma once
#include <boost/asio.hpp>
#include "tcp_connection.hpp"

using boost::asio::ip::tcp;

class tcp_server
{
public:
    tcp_server(boost::asio::io_context&);

    unsigned short get_port() const {
        return acceptor_.local_endpoint().port();
    }
private:
	void start_accept();
	void handle_accept(tcp_connection::shared_pointer, const boost::system::error_code&);

	tcp::acceptor acceptor_;
};
