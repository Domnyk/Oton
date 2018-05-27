#include "client.hpp"

client::client() : tcp_socket_(nullptr), is_connected_over_TCP(false),
    is_connected_over_UDP(false), ip_addr_("TEST") {

}

bool operator==(const client& a, const client& b) {
    return a.ip_addr_ == b.ip_addr_;
}

