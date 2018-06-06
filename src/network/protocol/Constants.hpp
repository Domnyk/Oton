#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "message_type.hpp"

namespace protocol {
/*
 *  Header length in bytes
 */
const unsigned int HEADER_LENGTH = 34;

/*
 *  Max body length in bytes
 */
const unsigned int MAX_BODY_LENGTH = 1024;
    class FieldLength {
    public:
        static const unsigned short msg_type = 2;
        static const unsigned short body_len = 8;
        static const unsigned short num_of_cols = 4;
        static const unsigned short num_of_rows = 4;
        static const unsigned short num_of_frames = 8;
        static const unsigned short frame_num = 8;
    };

    class FieldOffset {
    public:
        static const unsigned short msg_type = 0;
        static const unsigned short body_len = 2;
        static const unsigned short num_of_cols = 10;
        static const unsigned short num_of_rows = 14;
        static const unsigned short num_of_frames = 18;
        static const unsigned short frame_num = 26;
    };
}

#endif // CONSTANTS_H
