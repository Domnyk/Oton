#ifndef NETWORK_LAYER_HPP
#define NETWORK_LAYER_HPP

#include <string>
#include <boost/asio.hpp>
#include <vector>
#include <thread>
#include <QObject>
#include "../network/Acceptor.hpp"
#include "MovieList.hpp"

class NetworkLayer : public QObject
{
    Q_OBJECT

public:
    NetworkLayer(unique_ptr<MovieList>&,
                  const unsigned short max_num_of_clients = NetworkLayer::DEFAULT_MAX_NUM_OF_CLIENTS,
                  const unsigned short threads_num  = NetworkLayer::DEFAULT_NUM_OF_THREAD_FOR_ASIO);

    ~NetworkLayer();

    static const unsigned short DEFAULT_NUM_OF_THREAD_FOR_ASIO;
    static const unsigned short DEFAULT_MAX_NUM_OF_CLIENTS;

    unsigned short get_udp_port() const;
    unsigned short get_tcp_port() const;
signals:
    void user_connects(const std::string&);
    void user_disconnects(const std::string&);

    void server_closes();
private:
    const unsigned short threads_num_;
    unique_ptr<MovieList>& movie_list_;

    boost::asio::io_context io_context;
    Acceptor acceptor_;

    std::vector<std::thread> threads;
};

#endif
