#include "client.hpp"
#include <memory>

client::client() : is_connected_over_TCP(false), is_connected_over_UDP(false),
     tcp_socket_ptr_(nullptr), udp_remote_endpoint_ptr_(nullptr), ip_addr_() {

}

client::client(tcp::socket&& tcp_socket) : client() {
    /* auto temp_socket = new tcp::socket(
                tcp_socket.get_executor().context(),
                boost::asio::ip::tcp::v4(),
                tcp_socket.native_handle()
                ); */

    tcp_socket_ptr_ = std::make_unique<tcp::socket>(std::move(tcp_socket));
    ip_addr_ = tcp_socket_ptr_->remote_endpoint().address().to_string();
}

client::client(udp::endpoint&& udp_remote_endpoint): client() {
    udp_remote_endpoint_ptr_ = std::make_unique<udp::endpoint>(udp_remote_endpoint);
    ip_addr_ = udp_remote_endpoint_ptr_->address().to_string();
}

client::client(unique_ptr<udp::endpoint>&& udp_endpoint_ptr) : client() {
    udp_remote_endpoint_ptr_ = std::move(udp_endpoint_ptr);
}

bool operator==(const client& a, const client& b) {
    if(a.is_connected() && b.is_connected()) {
        return  a.ip_addr_ == b.ip_addr_ &&
                a.remote_tcp_port() == b.remote_tcp_port() &&
                a.remote_udp_port() == b.remote_udp_port();
    }

    return a.ip_addr_ == b.ip_addr_;
}

bool operator!=(const client& a, const client& b) {
    return !(a == b);
}

