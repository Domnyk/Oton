#include "gui/mainwindow.h"
#include <QApplication>

#include "../tests/test_example.h"
#include <QTest>

int main(int argc, char *argv[])
{
    test_example test;
    QTest::qExec(&test);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
