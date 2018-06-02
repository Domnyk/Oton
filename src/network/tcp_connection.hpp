#pragma once

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/io_context.hpp>
#include "movie_layer.hpp"
#include "protocol/Message.hpp"

using boost::asio::ip::tcp;

class tcp_connection {
public:
    typedef std::shared_ptr<tcp_connection> shared_pointer;

    static shared_pointer create(boost::asio::io_context&, unique_ptr<movie_layer>&);
    tcp::socket& get_socket();

    void read();
private:
    tcp_connection(boost::asio::io_context&, unique_ptr<movie_layer>&);

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

    protocol::Message message_;
    unique_ptr<movie_layer>& movie_layer_;
	tcp::socket socket_;
    std::string streamed_movie_;
};
