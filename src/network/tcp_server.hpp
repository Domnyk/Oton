#pragma once
#include <boost/asio.hpp>
#include <set>
#include "protocol/parser.hpp"
#include "tcp_connection.hpp"
#include "client.hpp"
#include "../movie/Movie.hpp"

using boost::asio::ip::tcp;

class tcp_server
{
public:
    tcp_server(boost::asio::io_context&, std::function<void(tcp::socket&)>, Movie&);

    unsigned int port() const {
        return acceptor_.local_endpoint().port();
    }
private:
    void start_accept();
    void handle_accept(tcp_connection::shared_pointer, boost::system::error_code&);

    void handle_send();

    tcp::acceptor acceptor_;
    Movie& test_movie_;
    std::function<void(tcp::socket&)> new_client_handler_;
};
