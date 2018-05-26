#include <iostream>
#include <boost/bind.hpp>
#include "udp_server.hpp"

const int ANY_PORT = 0;

udp_server::udp_server(boost::asio::io_context& io_context)
    : socket(io_context, udp::endpoint(udp::v4(), ANY_PORT))
{
    std::cout << "This shit is on port: " << socket.local_endpoint().port() << std::endl;
}

void udp_server::start_receive() {}

void udp_server::start_send(const std::string& address, unsigned short port, const std::string& msg) {
    std::cout << "start_send has been called" << std::endl;

    udp::endpoint remote_endpoint (
        boost::asio::ip::address::from_string(address),
        port
    );

    std::cout << "Remote endpoint address: " << remote_endpoint.address() << std::endl;
    std::cout << "Remote endpoint port: " << remote_endpoint.port() << std::endl;

    socket.async_send_to(boost::asio::buffer(msg),
        remote_endpoint,
        boost::bind(&udp_server::handle_send,
                    this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred
                   )
    );
}

void udp_server::handle_send(const boost::system::error_code& error,
    std::size_t bytes_transferred) {
    std::cout << "handle_send fired" << std::endl;
    std::cout << "Num of bytes transferred: " << bytes_transferred << std::endl;
}
