#include <QTest>
#include <iostream>
#include "../tests/HeaderTest.hpp"
#include "../tests/FrameTest.hpp"
#include "../tests/MsgTest.hpp"
#include "../tests/MovieListTest.hpp"
#include "../tests/MovieTest.hpp"

int main()
{
    MovieListTest movie_layer_test;
    HeaderTest header_test;
    MovieTest movie_test;
    FrameTest frame_test;
    MsgTest msg_test;

    return QTest::qExec(&movie_layer_test) |
           QTest::qExec(&header_test) |
           QTest::qExec(&movie_test) |
           QTest::qExec(&frame_test) |
           QTest::qExec(&msg_test);

}
