#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "Header.hpp"

namespace protocol {
    class message
    {
    public:
        static const int header_length = 34;
        static const int max_body_length = 1000000;

        const unsigned char* data() const;
        unsigned char* data();

        const unsigned char* body() const;
        unsigned char* body();

        message();
    private:
        unsigned char data_[header_length + max_body_length] = "";
        Header header_;
    };
}
#endif // MESSAGE_HPP
