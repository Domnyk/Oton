#ifndef TEST_CLIENT_HPP
#define TEST_CLIENT_HPP

#include <QObject>
#include <QtTest/QtTest>
#include <../src/network/client.hpp>

class test_client: public QObject
{
    Q_OBJECT

private slots:
    void when_clients_have_same_ip_compare_should_return_true();
    void when_clients_have_diffrent_ip_compare_should_return_false();
};

#endif // TEST_CLIENT_HPP
