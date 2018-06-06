#include <QTest>
#include <iostream>
#include "../tests/HeaderTest.hpp"

int main(int argc, char *argv[])
{
    HeaderTest header_test;

    return QTest::qExec(&header_test);
}
