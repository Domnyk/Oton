#ifndef SENDER_HPP
#define SENDER_HPP

#include <memory>
#include <boost/asio.hpp>
#include "protocol/Constants.hpp"

using namespace boost::asio::ip;

class Sender
{
public:
    Sender(tcp::socket&, udp::socket&, std::shared_ptr<char>);

    void send_header();
    void send_body_tcp(unsigned long);
    void send_body_udp(unsigned long);
    void send_confirmation(protocol::message_type);
    void send_with_confirmation_tcp(unsigned long, protocol::message_type);
    void send_with_confirmation_udp(unsigned long, protocol::message_type);

private:
    protocol::message_type read_confirmation();
    bool is_confirmation_correct(protocol::message_type, protocol::message_type) const;

    tcp::socket& tcp_socket_;
    udp::socket& udp_socket_;
    std::shared_ptr<char> data_;
};

#endif // SENDER_HPP
