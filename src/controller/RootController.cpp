#include "RootController.hpp"

RootController::RootController(int argc, char *argv[])
    : io_context_(), work_guard_(boost::asio::make_work_guard(io_context_)), threads_(),
      movie_list_(), acceptor_(movie_list_, io_context_), q_application_(argc, argv), main_window_(nullptr) {

    qRegisterMetaType<std::string>("std::string");

    QObject::connect(&main_window_, &MainWindow::file_has_been_choosed, this, &RootController::add_file);
    QObject::connect(this, &RootController::movie_already_on_list, &main_window_, &MainWindow::handle_movie_already_on_list);
    QObject::connect(this, &RootController::first_movie_added, &main_window_, &MainWindow::handle_first_movie_added);
    QObject::connect(this, &RootController::movie_added, &main_window_, &MainWindow::handle_movie_added);

    QObject::connect(&main_window_, &MainWindow::delete_has_been_clicked, this, &RootController::delete_file);
    QObject::connect(this, &RootController::last_movie_deleted, &main_window_, &MainWindow::handle_last_movie_deleted);

    QObject::connect(&main_window_, &MainWindow::start_distributing, &acceptor_, &Acceptor::handle_start_distributing);
    QObject::connect(&main_window_, &MainWindow::stop_distributing, &acceptor_, &Acceptor::handle_stop_distributing);
    QObject::connect(&acceptor_, &Acceptor::server_distributes, &main_window_, &MainWindow::handle_server_distributes);

    QObject::connect(&acceptor_, &Acceptor::user_connects, &main_window_, &MainWindow::user_connected);
    QObject::connect(&acceptor_, &Acceptor::user_disconnects, &main_window_, &MainWindow::user_disconnected);

    QObject::connect(&main_window_, &MainWindow::window_closes, this, &RootController::handle_window_closed);

    start_ioc_threads();

    main_window_.show();
}

int RootController::exec() const {
    return q_application_.exec();
}

void RootController::add_file(const QString filepath) {
    std::string _filepath = filepath.toStdString();

    bool movie_added = movie_list_.add_movie(_filepath);
    if(!movie_added) {
        emit movie_already_on_list();
        return;
    }

    QString filename = QString::fromStdString(Movie::get_filename(_filepath));

    const unsigned short num_of_movies = movie_list_.get_num_of_movies();
    if (num_of_movies == 1) {
        emit first_movie_added(filename);
        return;
    }

    emit this->movie_added(filename);
}

void RootController::delete_file(const QString filepath) {
    movie_list_.delete_movie(filepath.toStdString());

    const unsigned short num_of_movies = movie_list_.get_num_of_movies();
    if (num_of_movies == 0) {
        emit last_movie_deleted();
    }
}


void RootController::start_ioc_threads() {
    for (int i = 0; i < RootController::NUM_OF_THREADS; ++i) {
        threads_.emplace_back(
            [&]() {
                io_context_.run();
            }
        );
    }

    for(auto& thread: threads_) {
        thread.detach();
    }
 }

void RootController::stop_ioc_threads() {
    for(auto& thread: threads_) {
        thread.~thread();
    }
    threads_.clear();
}

void RootController::handle_window_closed() {
    io_context_.stop();
}


