#include "network_layer.hpp"

const std::string network_layer::default_address = "127.0.0.1";
const unsigned short network_layer::default_port = 6969;

network_layer::network_layer(const std::string& address, const unsigned short port)
    : address(address), port(port), io_context(),
      tcp_server_(io_context), udp_server_(io_context){

}

network_layer::network_layer(): network_layer(network_layer::default_address, network_layer::default_port) {
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
