#include <iostream>
#include <boost/bind.hpp>
#include "udp_server.hpp"

const int ANY_PORT = 0;

udp_server::udp_server(boost::asio::io_context& io_context,
                       std::function<void(client&)> new_client_handler)
    : socket(io_context, udp::endpoint(udp::v4(), ANY_PORT)),
      new_client_handler_(new_client_handler) {
    std::cout << "This shit is on port: " << socket.local_endpoint().port() << std::endl;
}

void udp_server::start_receive() {
    protocol::parser parser;
    udp::endpoint remote_endpoint;

    socket.async_receive_from(
        boost::asio::buffer(parser.data(), parser.header_length()),
        remote_endpoint,
        [&](boost::system::error_code ec, std::size_t bytes_recvd) {
            if(!ec && bytes_recvd > 0) {
                std::cout << "Received some data" << std::endl;
                handle_receive(parser, remote_endpoint);
            } else {
                start_receive();
            }
        }
    );

}

void udp_server::handle_receive(protocol::parser& parser, udp::endpoint& remote_endpoint) {
    using namespace protocol;
    header header_;

    try {
        header_ = parser.parse_header();
    } catch (std::invalid_argument& error) {
        std::cerr << "Could not parse header" << std::endl;
        start_receive();
    }

    std::cout << "Header type: " << header_.type() << std::endl;

    switch(header_.type()) {
        case CONNECT:
            handle_connect(remote_endpoint);
            break;
        default:
            std::cout << "Unkown header type" << std::endl;
    }

    start_receive();
}

void udp_server::handle_connect(udp::endpoint& remote_endpoint) {
    std::cout << "Client has connected over UDP" << std::endl;
    client client_;

    new_client_handler_(client_);
}

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

void udp_server::handle_send(const boost::system::error_code& /* error */,
    std::size_t bytes_transferred) {
    std::cout << "handle_send fired" << std::endl;
    std::cout << "Num of bytes transferred: " << bytes_transferred << std::endl;
}
