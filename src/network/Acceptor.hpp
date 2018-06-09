#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP

#include <boost/asio.hpp>
#include <set>
#include <memory>
#include <QObject>
#include "Connection.hpp"
#include "../server/MovieList.hpp"

using namespace boost::asio::ip;

class Acceptor : public QObject
{
    Q_OBJECT
public:
    Acceptor(boost::asio::io_context&, unique_ptr<MovieList>&, const unsigned short);

    unsigned short get_tcp_port() const;
    unsigned short get_udp_port() const;
signals:
    void user_connects(const std::string&);
    void user_disconnects(const std::string&);

    void server_closes();
private slots:
    void user_connected();
    void user_disconnected();
private:
    void tcp_start_accept();
    void tcp_handle_accept(shared_ptr<Connection>, const boost::system::error_code&);

    bool is_max_num_of_clients_reached() const;

    tcp::acceptor tcp_acceptor_;
    unique_ptr<MovieList>& movie_list_;
    const unsigned short max_num_of_clients_;
    unsigned short curr_num_of_clients_;
};

#endif
