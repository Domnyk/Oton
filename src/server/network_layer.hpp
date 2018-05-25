#ifndef NETWORK_LAYER_HPP
#define NETWORK_LAYER_HPP

#include <string>
#include <boost/asio.hpp>
#include "../network/tcp_server.hpp"
#include "../network/udp_server.hpp"

class network_layer
{
public:
    network_layer();
    network_layer(const std::string& address, const unsigned short port);

    static const std::string default_address;
    static const unsigned short default_port;
private:
    void start_accept();
    void start_accept_tcp();
    void start_accept_udp();
    void handle_accept();

    std::string address;
    unsigned short port;

    boost::asio::io_context io_context;

    tcp_server tcp_server_;
    udp_server udp_server_;
};

#endif // NETWORK_LAYER_HPP
