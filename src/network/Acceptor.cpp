#include <iostream>
#include <algorithm>
#include <boost/system/error_code.hpp>
#include "Acceptor.hpp"

const unsigned short Acceptor::DEFAULT_MAX_NUM_OF_CLIENTS = 5;

const unsigned short ANY_PORT = 0;

Acceptor::Acceptor(MovieList& movie_list, boost::asio::io_context& io_context,
                   const unsigned short max_num_of_clients) :
    movie_list_(movie_list), io_context_(io_context),
    tcp_acceptor_(io_context_), max_num_of_clients_(max_num_of_clients), connections_() {
}

void Acceptor::handle_start_distributing(unsigned short max_num_of_clients) {
    max_num_of_clients_ = max_num_of_clients;

    open_acceptor();
    start_tcp_accept();
}

void Acceptor::start_tcp_accept() {
    auto connection_ptr = make_shared<Connection>(tcp_acceptor_.get_executor().context(), movie_list_);

    // Signal handling
    QObject::connect(connection_ptr.get(), &Connection::user_connects, this, &Acceptor::user_connected);
    QObject::connect(connection_ptr.get(), &Connection::user_disconnects, this, &Acceptor::user_disconnected);

    // Signal chaining
    QObject::connect(connection_ptr.get(), &Connection::user_connects, this, &Acceptor::user_connects);
    QObject::connect(connection_ptr.get(), &Connection::user_disconnects, this, &Acceptor::user_disconnects);

    tcp_acceptor_.async_accept(connection_ptr->get_tcp_socket(),
                               [connection_ptr, this](const boost::system::error_code& ec) {
                                    if(ec.value() == boost::system::errc::operation_canceled) {
                                        return;
                                    }

                                    if(ec) {
                                        std::cerr << "Error during TCP async_accept: " << ec.message() << ". Returning" <<  std::endl;
                                        this->start_tcp_accept();
                                        return;
                                    }

                                    this->handle_tcp_accept(connection_ptr);
                               }
    );
}

void Acceptor::handle_tcp_accept(shared_ptr<Connection> connection_ptr) {
    if (curr_num_of_clients_ >= max_num_of_clients_) {
        std::cerr << "Reached max number of connections. Closing this socket" << std::endl;
        connection_ptr->get_tcp_socket().close();
        this->start_tcp_accept();
        return;
    }

    std::cerr << "Client accepted over TCP" << std::endl;

    connections_.push_back(connection_ptr);

    std::thread t([connection_ptr](){
        connection_ptr->start();
    });
    t.detach();

    this->start_tcp_accept();
}

void Acceptor::handle_stop_distributing() {
    for(auto elem: connections_) {
        elem->server_close_btn_clicked();
    }

    try {
        tcp_acceptor_.cancel();
        tcp_acceptor_.close();
    } catch (std::exception& err) {
        std::cerr << "Error during acceptor close: " << err.what() << std::endl;
    }
}

void Acceptor::open_acceptor() {
    try {
        tcp::endpoint endpoint(tcp::v4(), ANY_PORT);
        tcp_acceptor_.open(endpoint.protocol());
        tcp_acceptor_.bind(endpoint);
        tcp_acceptor_.listen();

        emit server_distributes(tcp_acceptor_.local_endpoint().port());
    } catch (std::exception& err) {
        std::cerr << "Error during acceptor open: " << err.what() << std::endl;
    }
}

unsigned short Acceptor::get_tcp_port() const {
    return tcp_acceptor_.local_endpoint().port();
}

void Acceptor::user_disconnected() {
     --curr_num_of_clients_;
}

void Acceptor::user_connected() {
     ++curr_num_of_clients_;
}
