#pragma once

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/io_context.hpp>
#include "protocol/parser.hpp"
#include "movie_layer.hpp"

using boost::asio::ip::tcp;

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
public:
    typedef std::shared_ptr<tcp_connection> shared_pointer;

    static shared_pointer create(boost::asio::io_context&, unique_ptr<movie_layer>&);
	tcp::socket& socket();

    void start_read();
private:
    tcp_connection(boost::asio::io_context&, unique_ptr<movie_layer>&);

    void start_read_body_get_frame();
    void handle_read_body_get_frame();

	void handle_write(const boost::system::error_code&, size_t);
    void handle_read(const boost::system::error_code&);


    protocol::parser parser_;
    unique_ptr<movie_layer>& movie_layer_;
	tcp::socket socket_;
};
