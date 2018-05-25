#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <boost/asio.hpp>

using boost::asio::ip::udp;

class udp_server
{
public:
    udp_server(boost::asio::io_context&);
private:
    udp::socket socket;
    udp::endpoint remote_endpoint;
};

#endif // UDP_SERVER_HPP
