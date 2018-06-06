#include <string>
#include <cstdlib>
#include <iostream>
#include <array>
#include "Header.hpp"
#include "Constants.hpp"

using namespace protocol;
using namespace std;

Header::Header() {
}

Header::Header(message_type message_type, unsigned int body_len) : msg_type_(message_type),
                                                                               body_len_(body_len) {
    std::cerr << "Fix Header::Header(char*, message_type, unsigned int)" << std::endl;
    encode();
}

Header::Header(message_type msg_type, unsigned int body_len,
               unsigned short num_of_cols, unsigned short num_of_rows,
               unsigned int num_of_frames, unsigned int frame_num) : msg_type_(msg_type),
                                                                     body_len_(body_len),
                                                                     num_of_cols_(num_of_cols),
                                                                     num_of_rows_(num_of_rows),
                                                                     num_of_frames_(num_of_frames),
                                                                     frame_num_(frame_num) {}

unsigned int Header::parse_field(unsigned short field_length, unsigned short field_offset, const std::string& str_data) const {
    return static_cast<unsigned int>(std::stoul(str_data.substr(field_offset, field_length)));
}

void Header::parse(const char* data) {
    string str_data(data, protocol::HEADER_LENGTH);

    msg_type_ = static_cast<protocol::message_type>(parse_field(FieldLength::msg_type, FieldOffset::msg_type, str_data));
    body_len_ = parse_field(FieldLength::body_len, FieldOffset::body_len, str_data);
    num_of_cols_ = parse_field(FieldLength::num_of_cols, FieldOffset::num_of_cols, str_data);
    num_of_rows_ = parse_field(FieldLength::num_of_rows, FieldOffset::num_of_rows, str_data);
    num_of_frames_ = parse_field(FieldLength::num_of_frames, FieldOffset::num_of_frames, str_data);
    frame_num_ = parse_field(FieldLength::frame_num, FieldOffset::frame_num, str_data);
}

std::string Header::encode_field(unsigned short field_length, unsigned int field_value) {
    std::string format_string = "%0" + std::to_string(field_length) + "u";
    char* field_buf = new char[field_length + 1];
    std::sprintf(field_buf, format_string.c_str(), field_value);

    auto str = string(field_buf);
    delete[] field_buf;

    return str;
}

std::string Header::encode() {
    string header;
    string msg_type = encode_field(FieldLength::msg_type, static_cast<unsigned>(msg_type_));
    string body_len = encode_field(FieldLength::body_len, body_len_);
    string num_of_cols = encode_field(FieldLength::num_of_cols, num_of_cols_);
    string num_of_rows = encode_field(FieldLength::num_of_rows, num_of_rows_);
    string num_of_frames = encode_field(FieldLength::num_of_frames, num_of_frames_);
    string frame_num = encode_field(FieldLength::frame_num, frame_num_);

    header = msg_type + body_len + num_of_cols + num_of_rows + num_of_frames + frame_num;
    return header;
}


message_type Header::get_msg_type() const {
    return msg_type_;
}

unsigned int Header::get_body_len() const {
    return body_len_;
}

unsigned short Header::get_num_of_cols() const {
    return num_of_cols_;
}

unsigned short Header::get_num_of_rows() const {
    return num_of_rows_;
}

unsigned int Header::get_num_of_frames() const {
    return num_of_frames_;
}

unsigned int Header::get_frame_num() const {
    return frame_num_;
}

void Header::set_msg_type(message_type msg_type) {
    msg_type_ = msg_type;
}

void Header::set_body_len(unsigned int body_len) {
    body_len_ = body_len;
}

void Header::set_num_of_cols(unsigned short num_of_cols) {
    num_of_cols_ = num_of_cols;
}

void Header::set_num_of_rows(unsigned short num_of_rows) {
    num_of_rows_ = num_of_rows;
}
void Header::set_num_of_frames(unsigned num_of_frames) {
    num_of_frames_ = num_of_frames;
}

void Header::set_frame_num(unsigned frame_num) {
    frame_num_ = frame_num;
}
