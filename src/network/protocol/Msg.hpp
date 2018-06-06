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
            std::cerr << "Fix Message::msg_len()" << std::endl;
            return 0;
        }

        void set_body(const std::string&);
        void set_body(unsigned char*);
    private:
        std::shared_ptr<char> data_;
        Header header_;
    };
}

#endif
