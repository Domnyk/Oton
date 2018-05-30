#include <string>
#include <cstdlib>
#include <iostream>
#include "Header.hpp"
#include "Constants.hpp"

using namespace protocol;
using namespace std;

Header::Header() : data_(nullptr) {
}

Header::Header(char* data) : data_(data) {
}

unsigned short Header::parse_field(unsigned short field_length, char* field_data) {
    unique_ptr<char> buffer = unique_ptr<char>(new char[field_length + 1]);
    std::strncat(buffer.get(), field_data, field_length);

    /*
     * Temporary string creation to use std::stoi().
     * stoi() is better than std::atoi();
     */
    return std::stoi(std::string(buffer.get()));
}

void Header::parse_msg_type() {
    unsigned short buffer = parse_field(FieldLength::msg_type, data_);

    msg_type_ = static_cast<message_type>(buffer);
}
