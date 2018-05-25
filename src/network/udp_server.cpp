#include <iostream>
#include "udp_server.hpp"

udp_server::udp_server(boost::asio::io_context& io_context)
    : socket(io_context, udp::endpoint(udp::v4(), 0))
{
    std::cout << "UDP server is listening on: " << socket.local_endpoint().port() << std::endl;
}
