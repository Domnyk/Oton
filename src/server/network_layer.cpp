#include <iostream>
#include "network_layer.hpp"

const unsigned short network_layer::default_num_of_thread_for_asio = 5;

network_layer::network_layer(const std::string& address,
    const unsigned short num_of_threads_for_asio)
    : address(address), num_of_threads_for_asio(num_of_threads_for_asio),
      io_context(), tcp_server_(io_context), udp_server_(io_context),
      threads() {

    for (int i = 0; i < num_of_threads_for_asio; ++i) {
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

void network_layer::start_accept() {
    /*
     * Start accept TCP connections
     */

    /*
     * Start accept UDP connections
     */
}

void network_layer::start_accept_tcp() {
}
