#include <boost/bind.hpp>
#include <iostream>
#include <thread>

#include "tcp_connection.hpp"
#include "tcp_server.hpp"
#include "client.hpp"

const int ANY_PORT = 0;

tcp_server::tcp_server(boost::asio::io_context& io_context,
                       std::function<void(tcp::socket&)> new_client_handler,
                       unique_ptr<movie_layer>& movie_layer)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), ANY_PORT)),
      movie_layer_(movie_layer),
      new_client_handler_(new_client_handler) {
    start_accept();
}

void tcp_server::start_accept() {
    auto handler = tcp_connection::create(acceptor_.get_executor().context(), movie_layer_);

    acceptor_.async_accept( handler->get_socket(),
                            [=](auto ec){
                                this->handle_accept(handler, ec);
                            });
}

void tcp_server::handle_accept(std::shared_ptr<tcp_connection> handler, boost::system::error_code& ec) {
    if (ec) {
        std::cerr << "Error in TCP handle_accept" << std::endl;
        return;
    }

    std::cerr << "Accepted client over TCP" << std::endl;

    // Connected client logic
    std::thread t([=](){
        handler->read();
    });
    t.detach();
    new_client_handler_(handler->get_socket());


    // Continiue accepting
    auto new_handler = tcp_connection::create(acceptor_.get_executor().context(), movie_layer_);
    acceptor_.async_accept( new_handler->get_socket(),
                            [=](auto ec){
                                this->handle_accept(new_handler, ec);
                            });
}

