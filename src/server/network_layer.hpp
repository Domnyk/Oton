#ifndef NETWORK_LAYER_HPP
#define NETWORK_LAYER_HPP

#include <string>
#include <boost/asio.hpp>
#include <vector>
#include <thread>
#include <QObject>
#include "../network/Acceptor.hpp"
#include "../network/client.hpp"
#include "movie_layer.hpp"

class network_layer : public QObject
{
    Q_OBJECT

public:
    network_layer(unique_ptr<movie_layer>&,
                  const unsigned short max_num_of_clients = network_layer::DEFAULT_MAX_NUM_OF_CLIENTS,
                  const unsigned short threads_num  = network_layer::DEFAULT_NUM_OF_THREAD_FOR_ASIO);

    ~network_layer();

    static const unsigned short DEFAULT_NUM_OF_THREAD_FOR_ASIO;
    static const unsigned short DEFAULT_MAX_NUM_OF_CLIENTS;

    void insert_new_client_tcp(tcp::socket&);

    unsigned short get_udp_port() const;
    unsigned short get_tcp_port() const;
signals:
    void user_connects(const std::string&);
private:
    const unsigned short threads_num_;
    unique_ptr<movie_layer>& movie_layer_;

    boost::asio::io_context io_context;
    Acceptor acceptor_;

    std::vector<std::thread> threads;
};

#endif // NETWORK_LAYER_HPP
