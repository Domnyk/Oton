#include "test_example.h"

void test_example::checkTrueStatement()
{
	int x = 2;
	
	QCOMPARE(x, 2);
}

void test_example::checkFalseStatement()
{
	int x = 3;

    QCOMPARE(x, 2);
}

//QTEST_MAIN(test_example)
#include "bin/moc/moc_test_example.cpp"
