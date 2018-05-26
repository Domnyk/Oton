#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "../server/Server.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_edit_host_address_textChanged(const QString &arg1);

    void on_btn_choose_file_clicked(bool checked);

    void on_btn_close_server_clicked();

    void on_btn_choose_file_clicked();

    void on_btn_delete_movie_clicked();

    void on_btn_kick_peer_clicked();

    void on_btn_show_details_clicked();

    void on_btn_open_server_clicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<Server> server;
};

#endif // MAINWINDOW_H
