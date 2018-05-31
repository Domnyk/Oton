#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "Header.hpp"

namespace protocol {
    class Message
    {
    public:
        static const int HEADER_LENGTH = 34;
        static const int MAX_BODY_LENGTH = 1000000;

        Message();
        Message(protocol::message_type, unsigned int body_len);

        const char* data() const;
        char* data();

        const char* body() const;
        char* body();

        Header& get_header();
        const Header& get_header() const;

        unsigned int msg_len() {
            return HEADER_LENGTH + header_.get_body_len();
        }

        void set_body(const std::string&);
    private:
        char data_[HEADER_LENGTH + MAX_BODY_LENGTH] = "";
        Header header_;
    };
}
#endif
