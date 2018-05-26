#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <boost/asio.hpp>

using boost::asio::ip::udp;

class udp_server
{
public:
    udp_server(boost::asio::io_context&);

    void start_send(const std::string&, unsigned short, const std::string&);

    unsigned short get_port() const {
        return socket.local_endpoint().port();
    }
private:
    void start_receive();
    void handle_receive();

    void handle_send(const boost::system::error_code&, std::size_t);


    udp::socket socket;
    udp::endpoint remote_endpoint;
};

#endif // UDP_SERVER_HPP
