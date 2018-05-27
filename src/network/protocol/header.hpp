#ifndef MESSAGE_HEADER_HPP
#define MESSAGE_HEADER_HPP

#include "message_type.hpp"

namespace protocol {
    class header
    {
    public:
        header();
        header(unsigned short);
        header(const char*);

        void parse_data();

        const char* data() const {
            return data_;
        }

        char* data() {
            return data_;
        }

        std::uint8_t length() const {
            return type_field_length;
        }

        unsigned int type() const {
            return type_;
        }

    private:
        char data_[type_field_length + 1];
        protocol::message_type type_;
    };
}


#endif // MESSAGE_HEADER_HPP
