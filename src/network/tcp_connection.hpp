#pragma once

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/io_context.hpp>

using boost::asio::ip::tcp;

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
public:
    typedef std::shared_ptr<tcp_connection> shared_pointer;

	static shared_pointer create(boost::asio::io_context&);
	tcp::socket& socket();

    void start_send(unsigned char* data, unsigned int data_length);
private:
	tcp_connection(boost::asio::io_context&);
	void handle_write(const boost::system::error_code&, size_t);
    void handle_send(const boost::system::error_code&);

	tcp::socket socket_;
};
