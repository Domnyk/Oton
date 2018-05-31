#ifndef MESSAGE_HEADER_HPP
#define MESSAGE_HEADER_HPP

#include "message_type.hpp"

namespace protocol {
    class Header
    {
    public:
        Header();
        Header(char*);
        Header(char*, protocol::message_type, unsigned int);
        Header(char*, protocol::message_type, unsigned int, unsigned short,
               unsigned short, unsigned int, unsigned int);

        message_type get_msg_type() const;
        unsigned int get_body_len() const;
        unsigned short get_num_of_cols() const;
        unsigned short get_num_of_rows() const;
        unsigned int get_num_of_frames() const;
        unsigned int get_frame_num() const;

        void set_msg_type(message_type);
        void set_body_len(unsigned int);
        void set_num_of_cols(unsigned short);
        void set_num_of_rows(unsigned short);
        void set_num_of_frames(unsigned);
        void set_frame_num(unsigned);

        void parse();
        void parse_msg_type();
        void parse_body_len();
        void parse_num_of_cols();
        void parse_num_of_rows();
        void parse_num_of_frames();
        void parse_frame_num();

        void encode();
    private:
        unsigned int parse_field(unsigned short, const char*);

        char* data_;
        message_type msg_type_ = static_cast<message_type>(0);
        unsigned int body_len_ = 0;
        unsigned short num_of_cols_ = 0;
        unsigned short num_of_rows_ = 0;
        unsigned int num_of_frames_ = 0;
        unsigned int frame_num_ = 0;
    };
}

#endif // MESSAGE_HEADER_HPP
