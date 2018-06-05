#include <string>
#include <cstdlib>
#include <iostream>
#include "Header.hpp"
#include "Constants.hpp"

using namespace protocol;
using namespace std;

Header::Header() {
}

Header::Header(char* data) {
    std::cerr << "Fix Header::Header(char*)" << std::endl;
}

Header::Header(char* data, message_type message_type, unsigned int body_len) : /*data_(data), */
                                                                               msg_type_(message_type),
                                                                               body_len_(body_len) {
    std::cerr << "Fix Header::Header(char*, message_type, unsigned int)" << std::endl;
    encode();
}

Header::Header(char* data, message_type msg_type, unsigned int body_len,
               unsigned short num_of_cols, unsigned short num_of_rows,
               unsigned int num_of_frames, unsigned int frame_num) : /* data_(data), */
                                                                     msg_type_(msg_type),
                                                                     body_len_(body_len),
                                                                     num_of_cols_(num_of_cols),
                                                                     num_of_rows_(num_of_rows),
                                                                     num_of_frames_(num_of_frames),
                                                                     frame_num_(frame_num) {
    std::cerr << "Fix Header constructor with a lot of parameters" << std::endl;
}

unsigned int Header::parse_field(unsigned short field_length, const char* field_data) {
    string data(field_data, field_length);
    return static_cast<unsigned int>(std::stoul(data));
}

void Header::parse(char* data) {
    parse_msg_type(data);
    parse_body_len(data);
    parse_num_of_cols(data);
    parse_num_of_rows(data);
    parse_num_of_frames(data);
    parse_frame_num(data);
}

void Header::parse_msg_type(char* data) {
    unsigned short buffer = parse_field(FieldLength::msg_type, data);

    msg_type_ = static_cast<message_type>(buffer);
}

void Header::parse_body_len(char* data) {
    const char* body_len_data = data + FieldLength::msg_type;
    body_len_ = parse_field(FieldLength::body_len, body_len_data);
}

void Header::parse_num_of_cols(char* data) {
    const char* num_of_cols_data = data +
                                   FieldLength::msg_type +
                                   FieldLength::body_len;
    num_of_cols_ = parse_field(FieldLength::num_of_cols, num_of_cols_data);
}

void Header::parse_num_of_rows(char* data) {
    const char* num_of_rows_data = data +
                                   FieldLength::msg_type +
                                   FieldLength::body_len +
                                   FieldLength::num_of_cols;
    num_of_rows_ = parse_field(FieldLength::num_of_rows, num_of_rows_data);
}

void Header::parse_num_of_frames(char* data) {
    const char* num_of_frames_data = data +
                                   FieldLength::msg_type +
                                   FieldLength::body_len +
                                   FieldLength::num_of_cols +
                                   FieldLength::num_of_rows;
    num_of_frames_ = parse_field(FieldLength::num_of_frames, num_of_frames_data);
}

void Header::parse_frame_num(char* data) {
    const char* frame_num_data = data +
                                   FieldLength::msg_type +
                                   FieldLength::body_len +
                                   FieldLength::num_of_cols +
                                   FieldLength::num_of_rows +
                                   FieldLength::num_of_frames;
    frame_num_ = parse_field(FieldLength::frame_num, frame_num_data);
}

std::string Header::encode() {
    string header;

    char msg_type_buf[FieldLength::msg_type + 1] = "";
    std::sprintf(msg_type_buf, "%02d", static_cast<int>(msg_type_));
    string msg_type(msg_type_buf);

    char body_len_buf[FieldLength::body_len + 1] = "";
    std::sprintf(body_len_buf, "%08u", body_len_);
    string body_len(body_len_buf);

    char num_of_cols_buf[FieldLength::num_of_cols + 1] = "";
    std::sprintf(num_of_cols_buf, "%04u", num_of_cols_);
    string num_of_cols(num_of_cols_buf);

    char num_of_rows_buf[FieldLength::num_of_rows + 1] = "";
    std::sprintf(num_of_rows_buf, "%04u", num_of_rows_);
    string num_of_rows(num_of_rows_buf);

    char num_of_frames_buf[FieldLength::num_of_frames + 1] = "";
    std::sprintf(num_of_frames_buf, "%08u", num_of_frames_);
    string num_of_frames(num_of_frames_buf);

    char frame_num_buf[FieldLength::frame_num + 1] = "";
    std::sprintf(frame_num_buf, "%08u", frame_num_);
    string frame_num(frame_num_buf);

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
