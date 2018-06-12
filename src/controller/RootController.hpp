#ifndef ROOTCONTROLLER_HPP
#define ROOTCONTROLLER_HPP

#include <QObject>
#include <QApplication>
#include <boost/asio.hpp>
#include <thread>
#include "../gui/mainwindow.h"
#include "../movie/MovieList.hpp"
#include "../network/Acceptor.hpp"

class RootController : public QObject
{
    Q_OBJECT
public:
    static const unsigned int NUM_OF_THREADS = 5;

    RootController(int, char**);

    int exec() const;

    void set_max_num_of_clients(unsigned short);
signals:
    void open_server_btn_clicked(unsigned short);

    void movie_already_on_list();
    void first_movie_added(const QString);
    void movie_added(const QString);

    void last_movie_deleted();

private slots:
    void handle_window_closed();

private:
    void add_file(const QString);
    void delete_file(const QString);

    void start_ioc_threads();
    void stop_ioc_threads();

    boost::asio::io_context io_context_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;

    std::vector<std::thread> threads_;

    MovieList movie_list_;

    Acceptor acceptor_;

    QApplication q_application_;
    MainWindow main_window_;

    unsigned short max_num_of_clients = 0;
};

#endif // ROOTCONTROLLER_HPP
