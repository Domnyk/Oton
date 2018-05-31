#include "ClientTest.hpp"

void ClientTest::when_clients_have_diffrent_ip_compare_should_return_false() {
    client client_1, client_2;
    client_1.set_ip_addr("192.168.10.100");
    client_2.set_ip_addr("192.168.20.200");

    QVERIFY(client_1 != client_2);
}

void ClientTest::when_clients_have_same_ip_compare_should_return_true() {
    client client_1, client_2;
    std::string ip_addr = "192.168.10.100";
    client_1.set_ip_addr(ip_addr);
    client_2.set_ip_addr(ip_addr);

    QVERIFY(client_1 == client_2);
}
