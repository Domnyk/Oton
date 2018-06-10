#include <iostream>
#include <algorithm>
#include <boost/system/error_code.hpp>
#include "NetworkLayer.hpp"

const unsigned short NetworkLayer::DEFAULT_NUM_OF_THREAD_FOR_ASIO = 5;
const unsigned short NetworkLayer::DEFAULT_MAX_NUM_OF_CLIENTS = 5;

const unsigned short ANY_PORT = 0;

NetworkLayer::NetworkLayer(MovieList& movie_list, const unsigned short max_num_of_clients,
                           const unsigned short threads_num) :
    threads_num_(threads_num), movie_list_(movie_list), io_context(),
    tcp_acceptor_(io_context), threads(), max_num_of_clients_(max_num_of_clients) {
}

void NetworkLayer::handle_start_distributing(unsigned short max_num_of_clients) {
    max_num_of_clients_ = max_num_of_clients;

    open_acceptor();
    start_tcp_accept();
    start_ioc_threads();
}

void NetworkLayer::start_tcp_accept() {
    auto connection_ptr = make_shared<Connection>(tcp_acceptor_.get_executor().context(), movie_list_);

    // Signal handling
    QObject::connect(&(*connection_ptr), &Connection::user_connects, this, &NetworkLayer::user_connected);
    QObject::connect(&(*connection_ptr), &Connection::user_disconnects, this, &NetworkLayer::user_disconnected);

    // Signal chaining
    QObject::connect(&(*connection_ptr), &Connection::user_connects, this, &NetworkLayer::user_connects);
    QObject::connect(&(*connection_ptr), &Connection::user_disconnects, this, &NetworkLayer::user_disconnects);
    // QObject::connect(this, &NetworkLayer::server_closes, &(*connection_ptr), &Connection::server_closes);

    std::cerr << "Is acceptor open: " << tcp_acceptor_.is_open() << std::endl;

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

void NetworkLayer::handle_tcp_accept(shared_ptr<Connection> connection_ptr) {
    if (curr_num_of_clients_ >= max_num_of_clients_) {
        std::cerr << "Reached max number of connections. Closing this socket" << std::endl;
        connection_ptr->get_tcp_socket().close();
        this->start_tcp_accept();
        return;
    }

    std::cerr << "Client accepted over TCP" << std::endl;

    std::thread t([connection_ptr](){
        connection_ptr->start();
    });
    t.detach();

    this->start_tcp_accept();
}

void NetworkLayer::handle_stop_distributing() {
    try {
        tcp_acceptor_.cancel();
        tcp_acceptor_.close();
    } catch (std::exception& err) {
        std::cerr << "Error during acceptor close: " << err.what() << std::endl;
    }

    stop_ioc_threads();
}

unsigned short NetworkLayer::get_tcp_port() const {
    return tcp_acceptor_.local_endpoint().port();
}

void NetworkLayer::user_disconnected() {
     --curr_num_of_clients_;
}

void NetworkLayer::user_connected() {
     ++curr_num_of_clients_;
}

NetworkLayer::~NetworkLayer() {
    // io_context.stop();
}

void NetworkLayer::open_acceptor() {
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

void NetworkLayer::start_ioc_threads() {
    for (int i = 0; i < threads_num_; ++i) {
        threads.emplace_back(
            [&]() {
                io_context.run();
            }
        );
    }

    for(auto& thread: threads) {
        thread.detach();
    }
 }

void NetworkLayer::stop_ioc_threads() {
    for(auto& thread: threads) {
        thread.~thread();
    }

    threads.clear();
}
