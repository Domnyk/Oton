#include <QTest>
#include <iostream>
#include "../tests/ClientTest.hpp"
#include "../tests/HeaderTest.hpp"

int main(int argc, char *argv[])
{
    ClientTest client_test;
    HeaderTest header_test;

    return QTest::qExec(&client_test) |
            QTest::qExec(&header_test);
}
