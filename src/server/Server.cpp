#include <iostream>
#include "Server.hpp"

Server::Server()
    : network_layer_(), movie_layer_() {
}

Server::Server(const std::string& address, unsigned short port)
    : network_layer_(address, port), movie_layer_()  {
}
