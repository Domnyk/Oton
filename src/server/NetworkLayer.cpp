#include <iostream>
#include <algorithm>
#include "NetworkLayer.hpp"

const unsigned short NetworkLayer::DEFAULT_NUM_OF_THREAD_FOR_ASIO = 5;
const unsigned short NetworkLayer::DEFAULT_MAX_NUM_OF_CLIENTS = 5;

NetworkLayer::NetworkLayer(unique_ptr<MovieList>& movie_list,
                             const unsigned short max_num_of_clients,
                             const unsigned short threads_num) :
    threads_num_(threads_num),
    movie_list_(movie_list),
    io_context(),
    acceptor_(io_context, movie_list_, max_num_of_clients),
    threads() {
    QObject::connect(&acceptor_, &Acceptor::user_connects, this, &NetworkLayer::user_connects);
    QObject::connect(&acceptor_, &Acceptor::user_disconnects, this, &NetworkLayer::user_disconnects);
    QObject::connect(this, &NetworkLayer::server_closes, &acceptor_, &Acceptor::server_closes);

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

NetworkLayer::~NetworkLayer() {
    io_context.stop();
}

unsigned short NetworkLayer::get_tcp_port() const {
    return acceptor_.get_tcp_port();
}
