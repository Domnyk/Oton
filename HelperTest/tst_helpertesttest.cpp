#include <QString>
#include <QtTest>

class HelperTestTest : public QObject
{
    Q_OBJECT

public:
    HelperTestTest();

private Q_SLOTS:
    void testCase1();
};

HelperTestTest::HelperTestTest()
{
}

void HelperTestTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(HelperTestTest)

#include "tst_helpertesttest.moc"
