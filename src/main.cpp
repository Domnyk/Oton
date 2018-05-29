#include "gui/mainwindow.h"
#include <QApplication>

#include "../tests/test_client.hpp"
#include <QTest>

int main(int argc, char *argv[])
{
    test_client test_client_;
    auto test_client_fails = static_cast<bool>(QTest::qExec(&test_client_));
    if (test_client_fails) {
        return 1;
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
