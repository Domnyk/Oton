#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include "Header.hpp"

namespace protocol {
    class Message
    {
    public:
        static const int HEADER_LENGTH = 34;
        static const int MAX_BODY_LENGTH = 1000000;

        Message();

        const char* data() const;
        char* data();

        const char* body() const;
        char* body();

        void set_data(const std::string&);

        Header& get_header();
        const Header& get_header() const;

        unsigned int msg_len() {
            return HEADER_LENGTH + header_.get_body_len();
        }

        void set_body(const std::string&);
        void set_body(unsigned char*);
    private:
        char data_[HEADER_LENGTH + MAX_BODY_LENGTH] = "";
        Header header_;
    };
}
#endif
