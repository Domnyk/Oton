#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <boost/asio.hpp>
#include <functional>
#include "client.hpp"
#include "protocol/parser.hpp"
#include "udp_connection.hpp"

class network_layer;

using boost::asio::ip::udp;

class udp_server {
public:
    udp_server(boost::asio::io_context&, std::function<void(udp::endpoint&)>);

    void start_send(const std::string&, unsigned short, const std::string&);

    void start_receive();

    unsigned int port() const {
        return socket_.local_endpoint().port();
    }

private:
    void handle_receive(boost::system::error_code&, std::size_t, shared_ptr<udp_connection>);

    void handle_send(const boost::system::error_code&, std::size_t);

    void handle_connect(udp::endpoint&);

    void handle_message(udp_connection::shared_pointer, protocol::Header&);


    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::function<void(udp::endpoint&)> new_client_handler_;
};

#endif // UDP_SERVER_HPP
