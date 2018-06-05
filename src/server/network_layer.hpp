#ifndef NETWORK_LAYER_HPP
#define NETWORK_LAYER_HPP

#include <string>
#include <boost/asio.hpp>
#include <vector>
#include <thread>
#include <QObject>
#include "../network/Acceptor.hpp"
#include "../network/udp_server.hpp"
#include "../network/client.hpp"
#include "movie_layer.hpp"

class network_layer : public QObject
{
    Q_OBJECT

public:
    network_layer(unique_ptr<movie_layer>&, const unsigned short threads_num  = network_layer::default_num_of_thread_for_asio);
    ~network_layer();

    static const unsigned short default_num_of_thread_for_asio;

    void insert_new_client(client&);
    void insert_new_client_udp(udp::endpoint&);
    void insert_new_client_tcp(tcp::socket&);

    unsigned short get_udp_port() const;
    unsigned short get_tcp_port() const;
signals:
    void user_connects(const std::string&);
private:
    void if_fully_connected_emit_user_connects_signal(client&);

    const unsigned short threads_num_;
    unique_ptr<movie_layer>& movie_layer_;

    boost::asio::io_context io_context;
    Acceptor acceptor_;

    std::vector<std::thread> threads;

    std::vector<client> clients;
};

#endif // NETWORK_LAYER_HPP
