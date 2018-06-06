#pragma once

#include <boost/asio.hpp>
#include <QObject>
#include "MovieLayer.hpp"
#include "protocol/Msg.hpp"

using namespace boost::asio::ip;

class Connection : public QObject {
    Q_OBJECT
public:
    Connection(boost::asio::io_context&, unique_ptr<MovieLayer>&);

    tcp::socket& get_tcp_socket();
    udp::socket& get_udp_socket();

    void start();
signals:
    void user_connects();
    void user_disconnects();
private:
    void read();

    void handle_get_movie_list();
    void handle_get_movie();
    void handle_get_frame();
    void handle_movie_finished();
    void handle_disconnect();

    void read_header();
    void read_body();
    protocol::message_type read_confirmation();
    void read_with_confirmation();

    void send_header();
    void send_body();
    void send_confirmation(protocol::message_type);
    void send_with_confirmation(protocol::message_type);

    bool is_confirmation_correct(protocol::message_type msg_type, protocol::message_type confirmation);

    protocol::Msg message_;
    unique_ptr<MovieLayer>& movie_layer_;
    tcp::socket tcp_socket_;
    udp::socket udp_socket_;
    unique_ptr<Movie> streamed_movie_;
};
