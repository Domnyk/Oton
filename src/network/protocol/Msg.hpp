#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <iostream>
#include "Header.hpp"

namespace protocol {
    class Msg
    {
    public:
        Msg();

        std::shared_ptr<const char> data() const;
        std::shared_ptr<char> data();

        std::shared_ptr<const char> body() const;
        std::shared_ptr<char> body();

        Header& get_header();
        const Header& get_header() const;

        unsigned int msg_len() {
            return protocol::HEADER_LENGTH + header_.get_body_len();
        }

        void set_header(std::string);

        void set_body(std::string);
        void set_body(unsigned char*);

        void parse_header();
    private:
        std::shared_ptr<char> data_;
        Header header_;
    };
}

#endif
