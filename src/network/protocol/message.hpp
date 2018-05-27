#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "header.hpp"

namespace protocol {
    class message
    {
    public:
        message();

        const char* header_data() const {
          return header_.data();
        }

        char* header_data() {
          return header_.data();
        }

        std::size_t header_length() const {
            return header_.length();
        }

        header& header() {
            return header_;
        }

        /*
         * Convience method
         */
        unsigned int type() const {
            return header_.type();
        }
    private:
        class header header_;
    };
}
#endif // MESSAGE_HPP
