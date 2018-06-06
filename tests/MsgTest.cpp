#include <QtTest/QtTest>
#include "MsgTest.hpp"
#include "../src/network/protocol/Msg.hpp"
#include "../src/network/protocol/Constants.hpp"

using namespace protocol;

void MsgTest::it_should_throw_exception_when_msg_body_size_is_bigger_than_max() {
    std::string too_long_string(HEADER_LENGTH + MAX_BODY_LENGTH + 100, 'A');
    Msg msg;

    QVERIFY_EXCEPTION_THROWN(msg.set_body(too_long_string), std::runtime_error);
}
