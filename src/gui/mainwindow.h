#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "../server/Server.hpp"

class RootController;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class RootController;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow* get_ui() {
        return ui;
    }

signals:
    void server_closes();

    void file_has_been_choosed(const QString);
    void delete_has_been_clicked(const QString);

    void start_distributing(unsigned short);
    void stop_distributing();

private slots:
    void handle_movie_already_on_list();
    void handle_first_movie_added(const QString);
    void handle_movie_added(const QString);

    void handle_last_movie_deleted();

    void handle_server_distributes(unsigned short);


    void on_edit_host_address_textChanged(const QString &arg1);

    void on_btn_choose_file_clicked(bool checked);

    void on_btn_close_server_clicked();

    void on_btn_choose_file_clicked();

    void on_btn_delete_movie_clicked();

    void on_btn_open_server_clicked();

    void user_connected(const std::string&);

    void user_disconnected(const std::string&);
private:
    Ui::MainWindow *ui;
};

#endif
