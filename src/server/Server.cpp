#include <iostream>
#include "Server.hpp"

Server::Server(const std::string& address)
    : network_layer_(address), movie_layer_()  {
}
