#ifndef UDP_CONNECTION_HPP
#define UDP_CONNECTION_HPP

#include <boost/asio.hpp>
#include "protocol/parser.hpp"

using boost::asio::ip::udp;

class udp_connection
{
public:
    typedef std::shared_ptr<udp_connection> shared_pointer;

    static shared_pointer create(boost::asio::io_context&);
    udp::endpoint& remote_endpoint() {
        return remote_endpoint_;
    }
    protocol::parser& parser() {
        return parser_;
    }
    boost::asio::mutable_buffer buffer() {
        return boost::asio::buffer(parser_.data(), parser_.header_length());
    }
private:
    udp_connection(boost::asio::io_context&);

    udp::endpoint remote_endpoint_;
    boost::asio::io_context& io_context_;
    protocol::parser parser_;
};

#endif // UDP_CONNECTION_HPP
