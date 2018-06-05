#include <string>
#include <iostream>
#include <cstring>
#include "Message.hpp"

using namespace protocol;

Message::Message() : header_() {

}

const char* Message::data() const {
    return data_;
}

char* Message::data() {
    return data_;
}

const char* Message::body() const {
    return data_ + Message::HEADER_LENGTH;
}

char* Message::body() {
    return data_ + Message::HEADER_LENGTH;
}

Header& Message::get_header() {
    return header_;
}

const Header& Message::get_header() const {
   return header_;
}

void Message::set_data(const std::string& data_str) {
    size_t str_size = data_str.size();
    if (str_size > Message::HEADER_LENGTH + Message::MAX_BODY_LENGTH) {
        throw std::runtime_error("Msg size too larger");
    }

    const char* data_c_str = data_str.c_str();
    strcpy(data_, data_c_str);
}

void Message::set_body(const std::string& str) {
    size_t str_size = str.size();


    if (str_size > Message::MAX_BODY_LENGTH) {
        throw std::runtime_error("Body size too larger");
    }

    header_.set_body_len(str_size);

    const char* c_str = str.c_str();
    strcpy(data_ + Message::HEADER_LENGTH, c_str);
}

void Message::set_body(unsigned char* data) {
    memcpy(data_ + Message::HEADER_LENGTH, data, header_.get_body_len());
}
