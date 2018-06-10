#ifndef ROOTCONTROLLER_HPP
#define ROOTCONTROLLER_HPP

#include <QObject>
#include <QApplication>
#include "../gui/mainwindow.h"
#include "../server/MovieList.hpp"
#include "../server/NetworkLayer.hpp"

class RootController : public QObject
{
    Q_OBJECT
public:
    RootController(int, char**);

    MovieList& get_movie_list();
    const MovieList& get_movie_list() const;

    NetworkLayer& get_network_layer();
    const NetworkLayer& get_network_layer() const;

    int exec() const;

    void set_max_num_of_clients(unsigned short);
signals:
    void open_server_btn_clicked(unsigned short);

    void movie_already_on_list();
    void first_movie_added(const QString);
    void movie_added(const QString);

    void last_movie_deleted();
private:
    void add_file(const QString);
    void delete_file(const QString);

    MovieList movie_list_;

    NetworkLayer network_layer_;

    QApplication q_application_;
    MainWindow main_window_;

    unsigned short max_num_of_clients = 0;
};

#endif // ROOTCONTROLLER_HPP
