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

class Acceptor : public QObject
{
    Q_OBJECT

    friend class RootController;

public:
    Acceptor(MovieList&, boost::asio::io_context&,
             const unsigned short max_num_of_clients = Acceptor::DEFAULT_MAX_NUM_OF_CLIENTS);

    static const unsigned short DEFAULT_MAX_NUM_OF_CLIENTS;

    unsigned short get_tcp_port() const;
signals:
    void user_connects(const std::string&);
    void user_disconnects(const std::string&);

    void server_distributes(unsigned short);

    void server_closes();

    void show_msg(int);
private slots:
    void user_connected();
    void user_disconnected();

    void handle_start_distributing(unsigned short);
    void handle_stop_distributing();
private:
    void start_tcp_accept();
    void handle_tcp_accept(shared_ptr<Connection>);

    void open_acceptor();

    MovieList& movie_list_;

    boost::asio::io_context& io_context_;

    tcp::acceptor tcp_acceptor_;

    vector<shared_ptr<Connection>> connections_;

    unsigned short max_num_of_clients_ = 1;
    unsigned short curr_num_of_clients_ = 0;
};

#endif
