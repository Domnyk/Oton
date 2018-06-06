#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <thread>

#include "Connection.hpp"
#include "Acceptor.hpp"
#include "client.hpp"

const int ANY_PORT = 0;

Acceptor::Acceptor(boost::asio::io_context& io_context,
                       std::function<void(tcp::socket&)> new_client_handler,
                       unique_ptr<movie_layer>& movie_layer)
    : tcp_acceptor_(io_context, tcp::endpoint(tcp::v4(), ANY_PORT)),
      movie_layer_(movie_layer),
      new_client_handler_(new_client_handler) {
    tcp_start_accept();
}

unsigned short Acceptor::get_tcp_port() const {
    return tcp_acceptor_.local_endpoint().port();
}

unsigned short Acceptor::get_udp_port() const {
    std::cerr << "Fix Acceptor::get_udp_port" << std::endl;
    return 0;
}

void Acceptor::tcp_start_accept() {
    auto connection_ptr = make_shared<Connection>(tcp_acceptor_.get_executor().context(), movie_layer_);

    tcp_acceptor_.async_accept(connection_ptr->get_tcp_socket(),
                               boost::bind(&Acceptor::tcp_handle_accept, this,
                                           connection_ptr, boost::asio::placeholders::error)
    );
}



void Acceptor::tcp_handle_accept(shared_ptr<Connection> connection_ptr, const boost::system::error_code& ec) {
    if (ec) {
        std::cerr << "Error during TCP async_accept: " << ec.message() << ". Returning" <<  std::endl;
        return;
    }
    std::cerr << "Client accepted over TCP" << std::endl;

    std::thread t([connection_ptr](){
        connection_ptr->start();
    });
    t.detach();

    /*
    std::thread t([=](){
        handler->read();
    });
    t.detach();
    new_client_handler_(handler->get_socket());
    */

    // Continiue accepting over TCP
    this->tcp_start_accept();
}

/* void Acceptor::udp_start_accept(shared_ptr<tcp::socket> tcp_socket_ptr) {
    boost::array<char, 1> data;
    auto remote_endpoint_ptr = make_shared<udp::endpoint>(tcp_socket_ptr->remote_endpoint().address(), ANY_PORT);

    udp_socket_.async_receive_from(boost::asio::buffer(data, data.size()),
                                   *remote_endpoint_ptr,
                                   boost::bind(&Acceptor::udp_handle_accept, this,
                                               tcp_socket_ptr, remote_endpoint_ptr,
                                               boost::asio::placeholders::error)
    );
} */

/* void Acceptor::udp_handle_accept(shared_ptr<tcp::socket> tcp_socket_ptr, shared_ptr<udp::endpoint> udp_endpoint_ptr,
                                 const boost::system::error_code& ec) {
    if (ec) {
        std::cerr << "Error during UDP async_receive_from: " << ec.message() << ". Returning" <<  std::endl;
        return;
    }

    std::cerr << "Client is fully connected. Waiting for message" << std::endl;
    std::thread t([this, tcp_socket_ptr, udp_endpoint_ptr](){
        tcp::socket tcp_socket(std::move(*tcp_socket_ptr));
        udp::socket udp_socket(udp_socket_.get_executor().context(), udp_socket_.local_endpoint());
        udp_socket.connect(*udp_endpoint_ptr);

        Connection connection(std::move(tcp_socket), std::move(udp_socket), movie_layer_);
        connection.read();
    });
    t.detach();

} */


