#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "message_type.hpp"

namespace protocol {
/*
 * This character is used to separate movies
 * when sending GIVE MOVIE LIST body
 */
const char MOVIE_DELIMITER = '\n';

/*
 *  Header length in bytes
 */
const unsigned int HEADER_LENGTH = 35;

/*
 *  Max body length in bytes
 */
const unsigned long MAX_BODY_LENGTH = 9999999;
    class FieldLength {
    public:
        static const unsigned short msg_type = 2;
        static const unsigned short body_len = 8;
        static const unsigned short num_of_cols = 4;
        static const unsigned short num_of_rows = 4;
        static const unsigned short num_of_frames = 8;
        static const unsigned short frame_num = 8;
        static const unsigned short is_key_frame = 1;
    };

    class FieldOffset {
    public:
        static const unsigned short msg_type = 0;
        static const unsigned short body_len = 2;
        static const unsigned short num_of_cols = 10;
        static const unsigned short num_of_rows = 14;
        static const unsigned short num_of_frames = 18;
        static const unsigned short frame_num = 26;
        static const unsigned short is_key_frame = 34;
    };
}

#endif // CONSTANTS_H
