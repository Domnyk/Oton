#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <boost/asio.hpp>

class client
{
public:
    typedef std::unique_ptr<boost::asio::ip::tcp::socket> tcp_socket;

    client();

    bool is_connected() const {
        return is_connected_over_TCP && is_connected_over_UDP;
    }

    tcp_socket& socket() {
        return tcp_socket_;
    }

    void set_ip_addr(const std::string& ip_addr) {
        ip_addr_ = ip_addr;
    }

    const std::string& get_ip_aadr() const {
        return ip_addr_;
    }

    friend bool operator==(const client& a, const client& b);

    bool is_connected_over_TCP;
    bool is_connected_over_UDP;
private:
    tcp_socket tcp_socket_;
    std::string ip_addr_;
};

#endif // CLIENT_HPP
