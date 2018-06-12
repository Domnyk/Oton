#pragma once

#include <boost/asio.hpp>
#include <QObject>
#include "MovieList.hpp"
#include "protocol/Msg.hpp"
#include "Reader.hpp"
#include "Sender.hpp"

using namespace boost::asio::ip;

class Connection : public QObject {
    Q_OBJECT
public:
    Connection(boost::asio::io_context&, MovieList&);

    tcp::socket& get_tcp_socket();

    void start();
signals:
    void user_connects(const std::string&);
    void user_disconnects(const std::string&);
public slots:
    void server_close_btn_clicked();
private:
    void communicate();
    bool handle_received_msg(protocol::message_type);

    bool handle_get_movie_list();
    bool handle_get_movie();
    bool handle_get_frame();
    bool handle_movie_finished();
    bool disconnect_client();

    void send_msg_with_frame(const Frame&, protocol::message_type);
    void send_frame(unsigned int);

    void prepare_header(protocol::message_type, unsigned, unsigned, unsigned, unsigned, unsigned, bool);

    protocol::Msg msg_;
    MovieList& movie_list_;
    tcp::socket tcp_socket_;
    udp::socket udp_socket_;
    unique_ptr<Movie> streamed_movie_;
    string id_string_;
    Reader reader_;
    Sender sender_;
};
