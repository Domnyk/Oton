#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <thread>

#include "tcp_connection.hpp"
#include "Acceptor.hpp"
#include "client.hpp"

const int ANY_PORT = 0;

Acceptor::Acceptor(boost::asio::io_context& io_context,
                       std::function<void(tcp::socket&)> new_client_handler,
                       unique_ptr<movie_layer>& movie_layer)
    : tcp_acceptor_(io_context, tcp::endpoint(tcp::v4(), ANY_PORT)),
      udp_socket_(io_context, udp::endpoint(udp::v4(), ANY_PORT)),
      movie_layer_(movie_layer),
      new_client_handler_(new_client_handler) {
    tcp_start_accept();
}

unsigned short Acceptor::get_tcp_port() const {
    return tcp_acceptor_.local_endpoint().port();
}

unsigned short Acceptor::get_udp_port() const {
    return udp_socket_.local_endpoint().port();
}

void Acceptor::tcp_start_accept() {
    auto tcp_socket_ptr = make_shared<tcp::socket>(tcp_acceptor_.get_executor().context());

    tcp_acceptor_.async_accept(*tcp_socket_ptr,
                               boost::bind(&Acceptor::tcp_handle_accept, this, tcp_socket_ptr, boost::asio::placeholders::error)
    );
}



void Acceptor::tcp_handle_accept(shared_ptr<tcp::socket> tcp_socket_ptr, const boost::system::error_code& ec) {
    if (ec) {
        std::cerr << "Error during TCP async_accept: " << ec.message() << ". Returning" <<  std::endl;
        return;
    }

    std::cerr << "Client accepted over TCP" << std::endl;
    std::cerr << "Initiating connection over UDP" << std::endl;

    // Continue listen on UDP to perform full connect
    udp_start_accept(tcp_socket_ptr);

    // Start listening for UDP connection


    /* udp_socket_.async_receive_from(boost::asio::buffer(data, data.size()),
                                   remote_endpoint,
                                   [](const boost::system::error_code& ec, std::size_t bytes_rcvd){
                                        if(ec) {
                                            std::cerr << "Error in lambda expression in handle_tcp_accept: " << ec.message() << std::endl;
                                        }

                                        std::cerr << "Returning for now" << std::endl;
                                   }
    ); */

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

void Acceptor::udp_start_accept(shared_ptr<tcp::socket> tcp_socket_ptr) {
    boost::array<char, 1> data;
    auto remote_endpoint_ptr = make_shared<udp::endpoint>(tcp_socket_ptr->remote_endpoint().address(), ANY_PORT);

    udp_socket_.async_receive_from(boost::asio::buffer(data, data.size()),
                                   *remote_endpoint_ptr,
                                   boost::bind(&Acceptor::udp_handle_accept, this,
                                               tcp_socket_ptr, remote_endpoint_ptr,
                                               boost::asio::placeholders::error)
    );
}

void Acceptor::udp_handle_accept(shared_ptr<tcp::socket> tcp_socket_ptr, shared_ptr<udp::endpoint> udp_endpoint_ptr,
                                 const boost::system::error_code& ec) {
    if (ec) {
        std::cerr << "Error during UDP async_receive_from: " << ec.message() << ". Returning" <<  std::endl;
        return;
    }

    std::cerr << "Is connected over TCP: " << tcp_socket_ptr->is_open() << std::endl;

    std::cerr << "Client is fully connected" << std::endl;
    std::cerr << "Test case: exiting" << std::endl;

    // Start client thread
    std::thread t([=](){

    });
    t.detach();
}


