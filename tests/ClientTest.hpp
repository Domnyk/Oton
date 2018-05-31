#ifndef CLIENTTEST_HPP
#define CLIENTTEST_HPP

#include <QtTest/QtTest>
#include <../src/network/client.hpp>

class ClientTest: public QObject
{
    Q_OBJECT

private slots:
    void when_clients_have_same_ip_compare_should_return_true();
    void when_clients_have_diffrent_ip_compare_should_return_false();
};

#endif
