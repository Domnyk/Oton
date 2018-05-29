#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <boost/asio.hpp>

using namespace boost::asio::ip;
using namespace std;

class client
{
public:
    /* typedef boost::asio::ip::udp::endpoint udp_endpoint;
    typedef std::unique_ptr<boost::asio::ip::tcp::socket> tcp_socket_ptr;
    typedef std::unique_ptr<udp_endpoint> udp_endpoint_ptr; */

    client();
    client(udp::endpoint&&);
    client(tcp::socket&&);
    client(unique_ptr<udp::endpoint>&&);

    bool is_connected() const {
        return is_connected_over_TCP && is_connected_over_UDP;
    }

    unique_ptr<tcp::socket>& tcp_socket_ptr() {
        return tcp_socket_ptr_;
    }

    unique_ptr<udp::endpoint>& udp_endpoint_ptr() {
        return udp_remote_endpoint_ptr_;
    }

    void set_ip_addr(const string& ip_addr) {
        ip_addr_ = ip_addr;
    }

    const string& get_ip_aadr() const {
        return ip_addr_;
    }

    unsigned int remote_tcp_port() const {
        return tcp_socket_ptr_->remote_endpoint().port();
    }

    unsigned int remote_udp_port() const {
        return udp_remote_endpoint_ptr_->port();
    };

    friend bool operator==(const client& a, const client& b);
    friend bool operator!=(const client& a, const client& b);

    bool is_connected_over_TCP;
    bool is_connected_over_UDP;
private:
    unique_ptr<tcp::socket> tcp_socket_ptr_;
    unique_ptr<udp::endpoint> udp_remote_endpoint_ptr_;
    string ip_addr_;
};

#endif // CLIENT_HPP
