#include <iostream>
#include "Server.hpp"

Server::Server() : network_layer_(nullptr), movie_layer_(nullptr) {}
Server::Server(MovieLayer* movie_layer) : network_layer_(nullptr), movie_layer_(movie_layer) {}

void Server::init_network_layer(const unsigned short max_num_of_clients) {
    if (network_layer_) {
        return;
    }

    network_layer_ = unique_ptr<network_layer>(new network_layer(movie_layer_, max_num_of_clients));
}
