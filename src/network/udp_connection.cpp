#include "udp_connection.hpp"

udp_connection::udp_connection(boost::asio::io_context& io_context) : io_context_(io_context) {
}

udp_connection::shared_pointer udp_connection::create(boost::asio::io_context& io_context) {
    return udp_connection::shared_pointer(new udp_connection(io_context));
}
