#ifndef PARSER_HPP
#define PARSER_HPP

#include <boost/asio.hpp>
#include "Constants.hpp"
#include "Header.hpp"

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

        boost::asio::streambuf& get_streambuf() {
            return streambuf_;
        }

        // protocol::Header parse_header();
    private:
        char data_[protocol::HEADER_LENGTH + protocol::MAX_BODY_LENGTH];
        boost::asio::streambuf streambuf_;
    };
}

#endif // PARSER_HPP
