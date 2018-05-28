#ifndef TEST_EXAMPLE_H
#define TEST_EXAMPLE_H

#include <QtTest/QtTest>


class test_example : public QObject
{
	Q_OBJECT
private slots:
	void checkTrueStatement();
	void checkFalseStatement();
};

#endif // TEST_EXAMPLE_H
