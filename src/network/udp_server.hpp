#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <boost/asio.hpp>
#include <functional>
#include "client.hpp"
#include "protocol/parser.hpp"

class network_layer;

using boost::asio::ip::udp;

class udp_server {
    using clients_t = std::vector<client>;

public:
    udp_server(boost::asio::io_context&, std::function<void(client&)>);

    void start_send(const std::string&, unsigned short, const std::string&);

    void start_receive();

    unsigned short get_port() const {
        return socket.local_endpoint().port();
    }
private:
    void handle_receive(protocol::parser&, udp::endpoint&);

    void handle_send(const boost::system::error_code&, std::size_t);

    void handle_connect(udp::endpoint&);


    udp::socket socket;
    std::function<void(client&)> new_client_handler_;
};

#endif // UDP_SERVER_HPP
