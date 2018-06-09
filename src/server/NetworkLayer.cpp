#include <iostream>
#include <algorithm>
#include "NetworkLayer.hpp"

const unsigned short NetworkLayer::DEFAULT_NUM_OF_THREAD_FOR_ASIO = 5;
const unsigned short NetworkLayer::DEFAULT_MAX_NUM_OF_CLIENTS = 5;

const unsigned short ANY_PORT = 0;

NetworkLayer::NetworkLayer(unique_ptr<MovieList>& movie_list, const unsigned short max_num_of_clients,
                           const unsigned short threads_num) :
    threads_num_(threads_num), movie_list_(movie_list), io_context(),
    tcp_acceptor_(io_context, tcp::endpoint(tcp::v4(), ANY_PORT)), threads(), max_num_of_clients_(max_num_of_clients) {

    start_tcp_accept();

    for (int i = 0; i < threads_num_; ++i) {
        threads.emplace_back(
            [&]() {
                io_context.run();
            }
        );
    }

    for(auto& thread: threads) {
        thread.detach();
    }
}

void NetworkLayer::start_tcp_accept() {
    auto connection_ptr = make_shared<Connection>(tcp_acceptor_.get_executor().context(), movie_list_);

    // Signal handling
    QObject::connect(&(*connection_ptr), &Connection::user_connects, this, &NetworkLayer::user_connected);
    QObject::connect(&(*connection_ptr), &Connection::user_disconnects, this, &NetworkLayer::user_disconnected);

    // Signal chaining
    QObject::connect(&(*connection_ptr), &Connection::user_connects, this, &NetworkLayer::user_connects);
    QObject::connect(&(*connection_ptr), &Connection::user_disconnects, this, &NetworkLayer::user_disconnects);
    QObject::connect(this, &NetworkLayer::server_closes, &(*connection_ptr), &Connection::server_closes);

    tcp_acceptor_.async_accept(connection_ptr->get_tcp_socket(),
                               [connection_ptr, this](const boost::system::error_code& ec) {
                                    if(ec) {
                                        std::cerr << "Error during TCP async_accept: " << ec.message() << ". Returning" <<  std::endl;
                                        this->start_tcp_accept();
                                        return;
                                    }

                                    this->handle_tcp_accept(connection_ptr);
                               }
    );

}

void NetworkLayer::handle_tcp_accept(shared_ptr<Connection> connection_ptr) {
    if (curr_num_of_clients_ >= max_num_of_clients_) {
        std::cerr << "Reached max number of connections. Closing this socket" << std::endl;
        connection_ptr->get_tcp_socket().close();
        this->start_tcp_accept();
        return;
    }

    std::cerr << "Client accepted over TCP" << std::endl;

    std::thread t([connection_ptr](){
        connection_ptr->start();
    });
    t.detach();

    this->start_tcp_accept();
}

unsigned short NetworkLayer::get_tcp_port() const {
    return tcp_acceptor_.local_endpoint().port();
}

void NetworkLayer::user_disconnected() {
     --curr_num_of_clients_;
}

void NetworkLayer::user_connected() {
     ++curr_num_of_clients_;
}

NetworkLayer::~NetworkLayer() {
    io_context.stop();
}
