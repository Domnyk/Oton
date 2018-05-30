#ifndef MESSAGE_HEADER_HPP
#define MESSAGE_HEADER_HPP

#include "message_type.hpp"

namespace protocol {
    class Header
    {
    public:
        Header();
        Header(char*);

        message_type get_msg_type() const;
        unsigned int get_body_len() const;
        unsigned short get_num_of_cols() const;
        unsigned short get_num_of_rows() const;
        unsigned int get_num_of_frames() const;
        unsigned int get_frame_num() const;
    private:
        unsigned short parse_field(unsigned short, char*);
        void parse_msg_type();
        void parse_body_len();
        void parse_num_of_cols();
        void parse_num_of_rows();
        void parse_num_of_frames();
        void parse_frame_num();

        char* data_;
        message_type msg_type_;
        unsigned int body_len_;
        unsigned short num_of_cols_;
        unsigned short num_of_rows_;
        unsigned int num_of_frames_;
        unsigned int frame_num_;
    };
}

#endif // MESSAGE_HEADER_HPP
