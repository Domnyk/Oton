#include <iostream>
#include <algorithm>
#include "network_layer.hpp"

const unsigned short network_layer::default_num_of_thread_for_asio = 5;

network_layer::network_layer(
    const std::string& address,
    const unsigned short num_of_threads_for_asio)
    : address(address),
      num_of_threads_for_asio(num_of_threads_for_asio),
      io_context(),
      tcp_server_(io_context),
      udp_server_(io_context, std::bind(&network_layer::insert_new_client, std::ref(*this), std::placeholders::_1)),
      threads(),
      clients() {

    std::cout << "network_layer constructor called" << std::endl;

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

    start_accept();
}

network_layer::~network_layer() {
    io_context.stop();
}

/*
 * When launched this seems not to work but it is only because I pass dumb data as client and emit event when I shouldnt
 */
void network_layer::insert_new_client(client& client) {
mutex.lock();
    std::cout << "handler called" << std::endl;

    auto it = std::find(clients.begin(), clients.end(), client);
    if (it == clients.end()) {
        std::cout << "exiting" << std::endl;
        clients.push_back(std::move(client));
        mutex.unlock();
        return;
    }

    // it->set_ip_addr("THIS WILL NOT WORK!!!!");

    if(! it->is_connected_over_UDP) {
        std::cout << "In UPD SECTION" << std::endl;
        emit user_connects(clients.back().get_ip_aadr());
        it->is_connected_over_UDP = true;
    }

    if(! it->is_connected_over_TCP) {
        it->socket() = std::move(client.socket());
        it->is_connected_over_TCP = true;
    }

    if (it->is_connected()) {
        emit user_connects(client.get_ip_aadr());
    }
mutex.unlock();
}

void network_layer::start_accept() {
    std::cout << "network_layer::start_accept has been called" << std::endl;

    /*
     * Start accept TCP connections
     */
    // tcp_server_.start_accept();

    /*
     * Start receiveing UDP connections
     */
    udp_server_.start_receive();
}
