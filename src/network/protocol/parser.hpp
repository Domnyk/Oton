#ifndef PARSER_HPP
#define PARSER_HPP

#include "constants.hpp"
#include "header.hpp"

namespace protocol {
    class parser
    {
    public:
        parser();

        const char* data() const {
            return data_;
        }

        char* data() {
            return data_;
        }

        unsigned int header_length() const {
            return protocol::HEADER_LENGTH;
        }

        protocol::header parse_header();
    private:
        char data_[protocol::HEADER_LENGTH + protocol::MAX_BODY_LENGTH];
    };
}

#endif // PARSER_HPP
