#include <string>
#include <cstdlib>
#include <iostream>
#include "header.hpp"

using namespace protocol;

header::header() {
}

header::header(unsigned short type)
    : type_(static_cast<protocol::message_type>(type)) {

}

void header::parse_data() {
    char message_type_[type_field_length + 1] = "";
    std::strncat(message_type_, data_, type_field_length);

    std::cout << message_type_ << std::endl;
    std::cout << std::atoi(message_type_) << std::endl;

    type_ = protocol::message_type(std::atoi(message_type_));
}
