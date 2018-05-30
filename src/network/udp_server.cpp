#include <iostream>
#include <boost/bind.hpp>
#include "udp_server.hpp"

const int ANY_PORT = 0;

udp_server::udp_server(boost::asio::io_context& io_context,
                       std::function<void(udp::endpoint&)> new_client_handler)
    : socket_(io_context, udp::endpoint(udp::v4(), ANY_PORT)),
      new_client_handler_(new_client_handler) {
    start_receive();
}

void udp_server::start_receive() {
    auto handler = udp_connection::create(socket_.get_executor().context());

    socket_.async_receive_from(
        handler->buffer(),
        handler->remote_endpoint(),
        [=](auto ec, std::size_t bytes_recvd) {
            handle_receive(ec, bytes_recvd, handler);
        }
    );

}

void udp_server::handle_receive(boost::system::error_code& ec, std::size_t bytes_recvd,
                                udp_connection::shared_pointer handler) {
    if(ec || bytes_recvd <= 0) {
        std::cerr << "Error while handle_receive";
        return;
    }

    protocol::Header header;

    /* try {
        header = handler->parser().parse_header();
    } catch (std::invalid_argument& error) {
        std::cerr << "Could not parse header received over UDP" << std::endl;
        start_receive();
        return;
    } */

    // handle_message(handler, header);
    start_receive();
}

void udp_server::handle_message(udp_connection::shared_pointer handler, protocol::Header& header) {
    std::cout << "Doing nothing in udp_server handle_message. FIX IT!" << std::endl;

    /* switch(header.type()) {
        case protocol::CONNECT:
            handle_connect(handler->remote_endpoint());
            break;
        default:
            std::cerr << "Unkown header type" << std::endl;
    } */
}

void udp_server::handle_connect(udp::endpoint& remote_endpoint) {
    new_client_handler_(remote_endpoint);
}
