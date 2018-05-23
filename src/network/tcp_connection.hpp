#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/io_context.hpp>

using boost::asio::ip::tcp;

class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
public:
	typedef boost::shared_ptr<tcp_connection> shared_pointer;

	static shared_pointer create(boost::asio::io_context&);
	tcp::socket& socket();
	void start();
private:
	tcp_connection(boost::asio::io_context&);
	void handle_write(const boost::system::error_code&, size_t);

	tcp::socket socket_;
	std::string message_;
};
