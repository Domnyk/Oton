#pragma once
#include <boost/asio.hpp>
#include <set>
#include "protocol/parser.hpp"
#include "tcp_connection.hpp"
#include "client.hpp"
#include "../server/movie_layer.hpp"

using boost::asio::ip::tcp;

class Acceptor
{
public:
    Acceptor(boost::asio::io_context&, std::function<void(tcp::socket&)>, unique_ptr<movie_layer>&);

    unsigned int get_tcp_port() const {
        return tcp_acceptor_.local_endpoint().port();
    }
private:
    void start_tcp_accept();
    void handle_tcp_accept(tcp_connection::shared_pointer, boost::system::error_code&);

    void start_udp_accept();

    tcp::acceptor tcp_acceptor_;
    udp::socket udp_socket_:
    unique_ptr<movie_layer>& movie_layer_;
    std::function<void(tcp::socket&)> new_client_handler_;
};
