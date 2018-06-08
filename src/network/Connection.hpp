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
    void user_connects(const std::string&);
    void user_disconnects(const std::string&);

    void server_closes();
public slots:
    void server_close_btn_clicked();
private:
    void read();

    bool handle_get_movie_list();
    bool handle_get_movie();
    bool handle_get_frame();
    bool handle_movie_finished();
    bool handle_disconnect();

    unsigned int read_header();
    void read_body();
    void read_with_confirmation();
    protocol::message_type read_confirmation();

    void send_msg_with_frame(const Frame&, protocol::message_type);
    void send_header();
    void send_body(tcp::socket&);
    void send_body(udp::socket&);
    void send_confirmation(protocol::message_type);
    void send_with_confirmation(tcp::socket&, protocol::message_type);
    void send_with_confirmation(udp::socket&, protocol::message_type);

    bool is_confirmation_correct(protocol::message_type msg_type, protocol::message_type confirmation);

    protocol::Msg message_;
    unique_ptr<MovieLayer>& movie_layer_;
    tcp::socket tcp_socket_;
    udp::socket udp_socket_;
    unique_ptr<Movie> streamed_movie_;
};
