#include <string>
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

void Message::set_body(const std::string& str) {
    size_t str_size = str.size();
    if (str_size > Message::MAX_BODY_LENGTH) {
        throw std::runtime_error("Body size too larger");
    }

    header_.set_body_len(str_size);

    const char* c_str = str.c_str();
    strcpy(data_ + Message::HEADER_LENGTH, c_str);
}
