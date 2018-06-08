#include <string>
#include <iostream>
#include <cstring>
#include "Msg.hpp"

using namespace protocol;

Msg::Msg() : data_(new char[protocol::HEADER_LENGTH + protocol::MAX_BODY_LENGTH]), header_() {
}

std::shared_ptr<const char> Msg::data() const {
    return data_;
}

std::shared_ptr<char> Msg::data() {
    return data_;
}

std::shared_ptr<const char> Msg::body() const {
    return std::shared_ptr<const char>(data_, data_.get() + HEADER_LENGTH);
}

std::shared_ptr<char> Msg::body() {
    return std::shared_ptr<char>(data_, data_.get() + HEADER_LENGTH);
}

Header& Msg::get_header() {
    return header_;
}

const Header& Msg::get_header() const {
   return header_;
}

void Msg::set_body(std::string str) {
    size_t str_size = str.size();


    if (str_size > MAX_BODY_LENGTH) {
        throw std::runtime_error("Body size too larger");
    }

    header_.set_body_len(str_size);

    const char* c_str = str.c_str();
    strcpy(body().get(), c_str);
}

void Msg::set_header(std::string str) {
    memcpy(data_.get(), str.c_str(), protocol::HEADER_LENGTH);
}

void Msg::set_body(unsigned char* data) {
    memcpy(data_.get() + HEADER_LENGTH, data, header_.get_body_len());
}
