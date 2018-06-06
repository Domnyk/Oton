#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <thread>

#include "Connection.hpp"
#include "Acceptor.hpp"
#include "client.hpp"

const int ANY_PORT = 0;

Acceptor::Acceptor(boost::asio::io_context& io_context,
                   unique_ptr<movie_layer>& movie_layer,
                   const unsigned short max_num_of_clients_)
    : tcp_acceptor_(io_context, tcp::endpoint(tcp::v4(), ANY_PORT)),
      movie_layer_(movie_layer),
      max_num_of_clients_(max_num_of_clients_),
      curr_num_of_clients_(0) {
    tcp_start_accept();
}

unsigned short Acceptor::get_tcp_port() const {
    return tcp_acceptor_.local_endpoint().port();
}

unsigned short Acceptor::get_udp_port() const {
    std::cerr << "Fix Acceptor::get_udp_port" << std::endl;
    return 0;
}

void Acceptor::tcp_start_accept() {
    auto connection_ptr = make_shared<Connection>(tcp_acceptor_.get_executor().context(), movie_layer_);
    QObject::connect(&(*connection_ptr), &Connection::user_connects, this, &Acceptor::user_connected);
    QObject::connect(&(*connection_ptr), &Connection::user_disconnects, this, &Acceptor::user_disconnected);

    tcp_acceptor_.async_accept(connection_ptr->get_tcp_socket(),
                               boost::bind(&Acceptor::tcp_handle_accept, this,
                                           connection_ptr, boost::asio::placeholders::error)
    );
}



void Acceptor::tcp_handle_accept(shared_ptr<Connection> connection_ptr, const boost::system::error_code& ec) {
    if (ec) {
        std::cerr << "Error during TCP async_accept: " << ec.message() << ". Returning" <<  std::endl;
        this->tcp_start_accept();
        return;
    }

    if(is_max_num_of_clients_reached()) {
        std::cerr << "Reached max number of connections. Closing this socket" << std::endl;
        connection_ptr->get_tcp_socket().close();
        this->tcp_start_accept();
        return;
    }

    std::cerr << "Client accepted over TCP" << std::endl;

    std::thread t([connection_ptr](){
        emit connection_ptr->user_connects();
        connection_ptr->start();
    });
    t.detach();

    /*
    std::thread t([=](){
        handler->read();
    });
    t.detach();
    new_client_handler_(handler->get_socket());
    */

    // Continiue accepting over TCP
    this->tcp_start_accept();
}

bool Acceptor::is_max_num_of_clients_reached() const {
    return curr_num_of_clients_ >= max_num_of_clients_;
}

void Acceptor::user_connected() {
    ++curr_num_of_clients_;
}

void Acceptor::user_disconnected() {
    --curr_num_of_clients_;
}


