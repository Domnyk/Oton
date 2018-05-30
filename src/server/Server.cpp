#include <iostream>
#include "Server.hpp"

Server::Server() : network_layer_(nullptr), movie_layer_(nullptr) {}
Server::Server(movie_layer* movie_layer) : network_layer_(nullptr), movie_layer_(movie_layer) {}

void Server::init_network_layer() {
    if (network_layer_) {
        return;
    }

    network_layer_ = unique_ptr<network_layer>(new network_layer(movie_layer_));
}
