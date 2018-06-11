#ifndef READER_HPP
#define READER_HPP

#include <QObject>
#include <boost/asio.hpp>
#include "protocol/Header.hpp"
#include "protocol/message_type.hpp"

using namespace boost::asio::ip;

class Reader
{
public:
    Reader(tcp::socket&, std::shared_ptr<char> data);

    void read_header();
    void read_body(unsigned long);
    void read_with_confirmation();
    protocol::message_type read_confirmation();

private:
    void send_confirmation(protocol::message_type);

    tcp::socket& tcp_socket_;
    std::shared_ptr<char> data_;
};

#endif
