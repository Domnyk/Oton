#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP

#include <boost/asio.hpp>
#include <set>
#include <memory>
#include <QObject>
#include "Connection.hpp"
#include "../server/MovieLayer.hpp"

using namespace boost::asio::ip;

class Acceptor : public QObject
{
    Q_OBJECT
public:
    Acceptor(boost::asio::io_context&, unique_ptr<MovieLayer>&, const unsigned short);

    unsigned short get_tcp_port() const;
    unsigned short get_udp_port() const;
private slots:
    void user_connected();
    void user_disconnected();
private:
    void tcp_start_accept();
    void tcp_handle_accept(shared_ptr<Connection>, const boost::system::error_code&);

    bool is_max_num_of_clients_reached() const;

    tcp::acceptor tcp_acceptor_;
    unique_ptr<MovieLayer>& movie_layer_;
    const unsigned short max_num_of_clients_;
    unsigned short curr_num_of_clients_;
};

#endif
