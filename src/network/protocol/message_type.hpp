#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H

#include <cstdint>

namespace protocol {
    enum message_type: std::uint8_t {
        CONNECT
    };

    enum { type_field_length = 8 };
}

#endif // MESSAGE_TYPE_H