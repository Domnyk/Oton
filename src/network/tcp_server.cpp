#include <boost/bind.hpp>
#include <iostream>

#include "tcp_connection.hpp"
#include "tcp_server.hpp"
#include "client.hpp"

const int ANY_PORT = 0;

tcp_server::tcp_server(boost::asio::io_context& io_context,
                       std::function<void(tcp::socket&)> new_client_handler,
                       Movie& test_movie)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), ANY_PORT)),
      new_client_handler_(new_client_handler),
      test_movie_(test_movie) {
    start_accept();
}

void tcp_server::start_accept() {
    auto handler = tcp_connection::create(acceptor_.get_executor().context());

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
    auto frame = test_movie_.videoStream().getFrame(10);

    std::cout << "Frame col: " << frame.getPixelMatrix().cols << std::endl;
    std::cout << "Frame row: " << frame.getPixelMatrix().rows << std::endl;
    std::cout << "Frame elemSize: " << frame.getPixelMatrix().elemSize() << std::endl;
    std::cout << "Frame data length: " << frame.data_length() << std::endl;

    handler->start_send(frame.data(), frame.data_length());
    new_client_handler_(handler->socket());

    start_accept();
}

