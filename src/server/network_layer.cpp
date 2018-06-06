#include <iostream>
#include <algorithm>
#include "network_layer.hpp"

const unsigned short network_layer::DEFAULT_NUM_OF_THREAD_FOR_ASIO = 5;
const unsigned short network_layer::DEFAULT_MAX_NUM_OF_CLIENTS = 5;

network_layer::network_layer(unique_ptr<MovieLayer>& movie_layer,
                             const unsigned short max_num_of_clients,
                             const unsigned short threads_num) :
    threads_num_(threads_num),
    movie_layer_(movie_layer),
    io_context(),
    acceptor_(io_context, movie_layer_, max_num_of_clients),
    threads() {
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

network_layer::~network_layer() {
    io_context.stop();
}

unsigned short network_layer::get_tcp_port() const {
    return acceptor_.get_tcp_port();
}

unsigned short network_layer::get_udp_port() const {
    return acceptor_.get_udp_port();
}

void network_layer::insert_new_client_tcp(tcp::socket& socket) {
    auto endpoint = socket.remote_endpoint();
    std::string client_desc =
            "TCP. " +
            endpoint.address().to_string() + ". " +
            to_string(endpoint.port());

    emit user_connects(client_desc);
}
