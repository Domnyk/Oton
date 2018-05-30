#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H

#include <cstdint>

namespace protocol {
    enum message_type: std::uint8_t {
        GET_MOVIE_LIST = 0,
        GIVE_MOVIE_LIST = 1,
        GET_MOVIE = 2,
        GIVE_FRAME = 3,
        GET_FRAME = 4,
        MOVIE_FINISHED = 5,
        DISCONNECT = 6
    };
}

#endif // MESSAGE_TYPE_H
