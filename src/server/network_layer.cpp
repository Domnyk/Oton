#include <iostream>
#include <algorithm>
#include "network_layer.hpp"

const unsigned short network_layer::default_num_of_thread_for_asio = 5;

network_layer::network_layer(unique_ptr<movie_layer>& movie_layer, const unsigned short threads_num) :
    threads_num_(threads_num),
    movie_layer_(movie_layer),
    io_context(),
    acceptor_(io_context, std::bind(&network_layer::insert_new_client_tcp, std::ref(*this), std::placeholders::_1), movie_layer_),
    threads(),
    clients() {
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

void network_layer::if_fully_connected_emit_user_connects_signal(client& client) {
    std::cout << "Is fully connected: " << client.is_connected() << std::endl;

    if(!client.is_connected()) return;

    std::cout << "Inside is_fully_conncted" << std::endl;
    std::cout << "remote endpoint is nullptr: " << (client.udp_endpoint_ptr().get() == nullptr) << std::endl;
    std::cout << "tcp socket is nullptr: " << (client.tcp_socket_ptr().get() == nullptr) << std::endl;

    std::string client_desc = client.get_ip_aadr() +
                              std::string(", TCP: ")  /* to_string(client.remote_tcp_port()) */ +
                              std::string(", UDP: ")  + to_string(client.remote_udp_port());


    emit user_connects(client_desc);
}

void network_layer::insert_new_client_udp(udp::endpoint& endpoint) {
    std::string client_desc =
            "UDP. " +
            endpoint.address().to_string() + ". " +
            to_string(endpoint.port());

    emit user_connects(client_desc);
}

void network_layer::insert_new_client_tcp(tcp::socket& socket) {
    auto endpoint = socket.remote_endpoint();
    std::string client_desc =
            "TCP. " +
            endpoint.address().to_string() + ". " +
            to_string(endpoint.port());

    emit user_connects(client_desc);
}
