#include <string>
#include <iostream>
#include <cstring>
#include "Message.hpp"

using namespace protocol;

Message::Message() : header_(data_) {

}

Message::Message(message_type message_type, unsigned int body_len) /*: header_(data_, message_type, body_len) */ {
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
    return data_ + Message::MAX_BODY_LENGTH;
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
    /* size_t str_size = str.size(),
           end_tag_size = Message::END_TAG.size();


    if (str_size > Message::MAX_BODY_LENGTH + end_tag_size) {
        throw std::runtime_error("Body size too larger");
    }

    header_.set_body_len(str_size + end_tag_size);

    std::string data_with_end_tag = str + Message::END_TAG;
    const char* c_str = data_with_end_tag.c_str();
    strcpy(data_ + Message::HEADER_LENGTH, c_str); */
}

void Message::set_body(char* data) {
    /* std::string str = std::string(data);
    str += Message::END_TAG;

    header_.set_body_len(str.size());

    strcpy(data_ + Message::HEADER_LENGTH, str.c_str()); */
}

void Message::set_body(unsigned char* data, unsigned int data_length) {
    /* memcpy(data_ + Message::HEADER_LENGTH, data, data_length);
    memcpy(data_ + Message::HEADER_LENGTH + data_length, Message::END_TAG.c_str(), 2);
    header_.set_body_len(data_length + 2); */
}
