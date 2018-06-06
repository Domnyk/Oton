#include <string>
#include <iostream>
#include <cstring>
#include "Msg.hpp"

using namespace protocol;

Msg::Msg() : header_() {

}

const char* Msg::data() const {
    return data_;
}

char* Msg::data() {
    return data_;
}

const char* Msg::body() const {
    return data_ + HEADER_LENGTH;
}

char* Msg::body() {
    return data_ + HEADER_LENGTH;
}

Header& Msg::get_header() {
    return header_;
}

const Header& Msg::get_header() const {
   return header_;
}

void Msg::set_body(const std::string& str) {
    size_t str_size = str.size();


    if (str_size > MAX_BODY_LENGTH) {
        throw std::runtime_error("Body size too larger");
    }

    header_.set_body_len(str_size);

    const char* c_str = str.c_str();
    strcpy(data_ + HEADER_LENGTH, c_str);
}

void Msg::set_body(unsigned char* data) {
    memcpy(data_ + HEADER_LENGTH, data, header_.get_body_len());
}