#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace protocol {
/*
 *  Header length in bytes
 */
const unsigned int HEADER_LENGTH = 2;

/*
 *  Max body length in bytes
 */
const unsigned int MAX_BODY_LENGTH = 1024;
    class FieldLength {
    public:
        static const unsigned short  msg_type = 2;
        static const unsigned short body_len = 8;
        static const unsigned short num_of_cols = 4;
        static const unsigned short num_of_rows = 4;
        static const unsigned short num_of_frames = 8;
        static const unsigned short frame_num = 8;
    };
}

#endif // CONSTANTS_H
