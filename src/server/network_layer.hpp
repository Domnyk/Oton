#ifndef NETWORK_LAYER_HPP
#define NETWORK_LAYER_HPP

#include <string>
#include <boost/asio.hpp>
#include <vector>
#include <thread>
#include "../network/tcp_server.hpp"
#include "../network/udp_server.hpp"

class network_layer
{
public:
    network_layer(const std::string&, const unsigned short num_of_threads_for_asio = network_layer::default_num_of_thread_for_asio);
    ~network_layer();

    static const unsigned short default_num_of_thread_for_asio;

    const udp_server& get_udp_server() const {
        return udp_server_;
    }

    const tcp_server& get_tcp_server() const {
        return tcp_server_;
    }
private:
    void start_accept();
    void start_accept_tcp();
    void start_accept_udp();
    void handle_accept();

    std::string address;
    unsigned short port;
    const unsigned short num_of_threads_for_asio;

    boost::asio::io_context io_context;

    tcp_server tcp_server_;
    udp_server udp_server_;

    std::vector<std::thread> threads;
};

#endif // NETWORK_LAYER_HPP
