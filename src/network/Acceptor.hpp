#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP

#include <boost/asio.hpp>
#include <set>
#include <memory>
#include "protocol/parser.hpp"
#include "tcp_connection.hpp"
#include "client.hpp"
#include "../server/movie_layer.hpp"

using namespace boost::asio::ip;

class Acceptor
{
public:
    Acceptor(boost::asio::io_context&, std::function<void(tcp::socket&)>, unique_ptr<movie_layer>&);

    unsigned short get_tcp_port() const;
    unsigned short get_udp_port() const;
private:
    void tcp_start_accept();
    void tcp_handle_accept(shared_ptr<tcp::socket>, const boost::system::error_code&);
    void udp_start_accept(shared_ptr<tcp::socket>);
    void udp_handle_accept(shared_ptr<tcp::socket>, shared_ptr<udp::endpoint>, const boost::system::error_code&);

    tcp::acceptor tcp_acceptor_;
    udp::socket udp_socket_;
    unique_ptr<movie_layer>& movie_layer_;
    std::function<void(tcp::socket&)> new_client_handler_;
};

#endif
