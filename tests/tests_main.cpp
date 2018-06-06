#include <QTest>
#include <iostream>
#include "../tests/HeaderTest.hpp"
#include "../tests/FrameTest.hpp"
#include "../tests/MsgTest.hpp"

int main(int argc, char *argv[])
{
    HeaderTest header_test;
    FrameTest frame_test;
    MsgTest msg_test;

    return QTest::qExec(&header_test) |
           QTest::qExec(&frame_test) |
           QTest::qExec(&msg_test);
}
