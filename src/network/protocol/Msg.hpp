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

        const char* data() const;
        char* data();

        const char* body() const;
        char* body();

        Header& get_header();
        const Header& get_header() const;

        unsigned int msg_len() {
            std::cerr << "Fix Message::msg_len()" << std::endl;
            return 0;
        }

        void set_body(const std::string&);
        void set_body(unsigned char*);
    private:
        char* data_;
        Header header_;
    };
}

#endif
