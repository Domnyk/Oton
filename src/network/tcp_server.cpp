#include <boost/bind.hpp>
#include <iostream>

#include "tcp_connection.hpp"
#include "tcp_server.hpp"
#include "client.hpp"

const int ANY_PORT = 0;

tcp_server::tcp_server(boost::asio::io_context& io_context,
                       std::function<void(tcp::socket&)> new_client_handler,
                       unique_ptr<movie_layer>& movie_layer)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), ANY_PORT)),
      new_client_handler_(new_client_handler),
      movie_layer_(movie_layer) {
    start_accept();
}

void tcp_server::start_accept() {
    auto handler = tcp_connection::create(acceptor_.get_executor().context(), movie_layer_);

    acceptor_.async_accept( handler->socket(),
                            [=](auto ec){
                                handle_accept(handler, ec);
                            });
}

void tcp_server::handle_accept(std::shared_ptr<tcp_connection> handler, boost::system::error_code& ec) {
    if (ec) {
        std::cerr << "Error in TCP handle_accept" << std::endl;
        return;
    }

    std::cout << "Accepted client over TCP" << std::endl;

    // Do logic
    handler->start_read();
    new_client_handler_(handler->socket());

    auto new_handler = tcp_connection::create(acceptor_.get_executor().context(), movie_layer_);

    acceptor_.async_accept( new_handler->socket(),
                            [=](auto ec){
                                handle_accept(new_handler, ec);
                            });

}

