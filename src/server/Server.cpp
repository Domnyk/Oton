#include <iostream>
#include "Server.hpp"

Server::Server() : network_layer_(nullptr), movie_list_(nullptr) {}
Server::Server(MovieList* movie_list) : network_layer_(nullptr), movie_list_(movie_list) {}

/* void Server::init_network_layer(const unsigned short max_num_of_clients) {
    if (network_layer_) {
        return;
    }

    network_layer_ = unique_ptr<NetworkLayer>(new NetworkLayer(movie_list_, max_num_of_clients));
} */
