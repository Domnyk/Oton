#include "Reader.hpp"


Reader::Reader(tcp::socket& tcp_socket, std::shared_ptr<char> data) : tcp_socket_(tcp_socket), data_(data) {

}

void Reader::read_header() {
    boost::asio::read(tcp_socket_, boost::asio::buffer(data_.get(), protocol::HEADER_LENGTH));
}

void Reader::read_body(unsigned long body_len) {
    boost::asio::read(tcp_socket_, boost::asio::buffer(data_.get() + protocol::HEADER_LENGTH, body_len));
}

void Reader::read_with_confirmation() {
    read_header();
    protocol::Header header(data_.get());

    bool is_confirmation_required = header.get_msg_type() == protocol::message_type::GET_MOVIE;
    if(is_confirmation_required) {
        send_confirmation(header.get_msg_type());
        read_body(header.get_body_len());
    }
}

protocol::message_type Reader::read_confirmation() {
    char confirmation[protocol::FieldLength::msg_type + 1] = "";

    boost::asio::read(tcp_socket_, boost::asio::buffer(confirmation, protocol::FieldLength::msg_type));

    auto confirmation_num = std::stoi(std::string(confirmation));
    return static_cast<protocol::message_type>(confirmation_num);
}

void Reader::send_confirmation(protocol::message_type msg_type) {
    std::string confirmation = "0" + std::to_string(msg_type);
    boost::asio::write(tcp_socket_, boost::asio::buffer(confirmation.c_str(), protocol::FieldLength::msg_type));
}

