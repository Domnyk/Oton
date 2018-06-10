#ifndef NETWORK_LAYER_HPP
#define NETWORK_LAYER_HPP

#include <string>
#include <boost/asio.hpp>
#include <vector>
#include <thread>
#include <QObject>
#include "MovieList.hpp"
#include "../network/Connection.hpp"

using namespace boost::asio::ip;

class RootController;

class NetworkLayer : public QObject
{
    Q_OBJECT

    friend class RootController;

public:
    NetworkLayer(MovieList&,
                  const unsigned short max_num_of_clients = NetworkLayer::DEFAULT_MAX_NUM_OF_CLIENTS,
                  const unsigned short threads_num  = NetworkLayer::DEFAULT_NUM_OF_THREAD_FOR_ASIO);

    ~NetworkLayer();

    static const unsigned short DEFAULT_NUM_OF_THREAD_FOR_ASIO;
    static const unsigned short DEFAULT_MAX_NUM_OF_CLIENTS;

    unsigned short get_tcp_port() const;
signals:
    void user_connects(const std::string&);
    void user_disconnects(const std::string&);

    void server_distributes(unsigned short);

    void server_closes();
private slots:
    void user_connected();
    void user_disconnected();

    void handle_start_distributing(unsigned short);
    void handle_stop_distributing();
private:
    void start_tcp_accept();
    void handle_tcp_accept(shared_ptr<Connection>);

    void start_ioc_threads();
    void stop_ioc_threads();

    void open_acceptor();

    const unsigned short threads_num_;
    MovieList& movie_list_;

    boost::asio::io_context io_context;

    std::vector<std::thread> threads;

    tcp::acceptor tcp_acceptor_;

    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;

    unsigned short max_num_of_clients_ = 1;
    unsigned short curr_num_of_clients_ = 0;
};

#endif
