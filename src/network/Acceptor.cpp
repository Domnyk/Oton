#include <boost/bind.hpp>
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
    start_tcp_accept();
}

void Acceptor::start_tcp_accept() {
    tcp::socket socket;

    tcp_acceptor_.async_accept(socket,
                               [&socket](auto ec){
                               if(ec) {
                                    std::cerr << "Error in lambda expression in start_tcp_accept" << std::endl;
                               }

                               this->handle_tcp_accept(handler, ec);
                            });
}

void Acceptor::handle_tcp_accept(std::shared_ptr<tcp_connection> handler, boost::system::error_code& ec) {
    std::cerr << "Accepted client over TCP" << std::endl;
    std::cerr << "Started listening over UDP" << std::endl;


    // Start listening for UDP connection



    /*
    std::thread t([=](){
        handler->read();
    });
    t.detach();
    new_client_handler_(handler->get_socket());
    */

    // Continiue accepting
    auto new_handler = tcp_connection::create(acceptor_.get_executor().context(), movie_layer_);
    acceptor_.async_accept( new_handler->get_socket(),
                            [=](auto ec){
                                this->handle_accept(new_handler, ec);
                            });
}
