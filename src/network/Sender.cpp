#include "Sender.hpp"


Sender::Sender(tcp::socket& tcp_socket, udp::socket& udp_socket, std::shared_ptr<char> data)
    : tcp_socket_(tcp_socket), udp_socket_(udp_socket), data_(data) {
}

void Sender::send_header() {
    boost::asio::write(tcp_socket_, boost::asio::buffer(data_.get(), protocol::HEADER_LENGTH));
}

void Sender::send_body_tcp(unsigned long body_len) {
    boost::asio::write(tcp_socket_, boost::asio::buffer(data_.get() + protocol::HEADER_LENGTH, body_len));
}

void Sender::send_body_udp(unsigned long body_len) {
    unsigned num_of_full_packets = body_len / protocol::MAX_DATAGRAM_SIZE;
    unsigned last_packet_size = body_len - (num_of_full_packets * protocol::MAX_DATAGRAM_SIZE);

    for (unsigned short i = 0; i < num_of_full_packets; ++i) {
        udp_socket_.send(boost::asio::buffer(data_.get() + protocol::HEADER_LENGTH + protocol::MAX_DATAGRAM_SIZE * i, protocol::MAX_DATAGRAM_SIZE));
    }

    udp_socket_.send(boost::asio::buffer(data_.get() + protocol::HEADER_LENGTH + protocol::MAX_DATAGRAM_SIZE * (num_of_full_packets), last_packet_size));
}

/*
 * This method is here for simplicity
 */
protocol::message_type Sender::read_confirmation() {
    char confirmation[protocol::FieldLength::msg_type + 1] = "";

    boost::asio::read(tcp_socket_, boost::asio::buffer(confirmation, protocol::FieldLength::msg_type));

    auto confirmation_num = std::stoi(std::string(confirmation));
    return static_cast<protocol::message_type>(confirmation_num);
}

void Sender::send_confirmation(protocol::message_type msg_type) {
    std::string confirmation;
    if (static_cast<unsigned>(msg_type) > 9) {
        confirmation = std::to_string(msg_type);
    } else {
        confirmation = "0" + std::to_string(msg_type);
    }

    boost::asio::write(tcp_socket_, boost::asio::buffer(confirmation.c_str(), protocol::FieldLength::msg_type));
}

void Sender::send_with_confirmation_tcp(unsigned long body_len, protocol::message_type expected_confirmation) {
    send_header();

    protocol::message_type actual = read_confirmation();
    if(!is_confirmation_correct(actual, expected_confirmation)) {
        throw std::runtime_error("Wrong confirmation number");
    }

    send_body_tcp(body_len);
}

void Sender::send_with_confirmation_udp(unsigned long body_len, protocol::message_type expected_confirmation) {
    send_header();

    protocol::message_type actual = read_confirmation();
    if(!is_confirmation_correct(actual, expected_confirmation)) {
        throw std::runtime_error("Wrong confirmation number");
    }

    send_body_udp(body_len);
}

bool Sender::is_confirmation_correct(protocol::message_type actual, protocol::message_type expected) const {
    return actual == expected;
}


