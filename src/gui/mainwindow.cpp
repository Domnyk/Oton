#include <iostream>
#include <string>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_edit_host_address_textChanged(const QString &arg1)
{

}

void MainWindow::on_btn_choose_file_clicked(bool checked)
{
    ui->label_ip_address->setText(QFileDialog::getOpenFileName(this, "test", QString(), tr("Movies (*.h *.cpp)")));
}

void MainWindow::on_btn_close_server_clicked()
{
    // Destory object under server unique_ptr
    server.reset(nullptr);

    // clear udp_port, tcp_port
    ui->udp_port->clear();
    ui->tcp_port->clear();

    // clear list of movies and list of peers
    ui->list_movies->clear();
    ui->list_peers->clear();

    // Enable start server btn
    ui->btn_open_server->setEnabled(true);
}

void MainWindow::on_btn_choose_file_clicked()
{

}

void MainWindow::on_btn_delete_movie_clicked()
{

}

void MainWindow::on_btn_kick_peer_clicked()
{

}

void MainWindow::on_btn_show_details_clicked()
{

}

void MainWindow::on_btn_open_server_clicked()
{
    ui->btn_open_server->setEnabled(false);
    ui->btn_close_server->setEnabled(true);
    std::string host_address = ui->edit_host_address->text().toStdString();

    server = std::unique_ptr<Server>(new Server(host_address));    

    unsigned short udp_port = server->get_udp_port();
    unsigned short tcp_port = server->get_tcp_port();
    ui->udp_port->setText(QString::fromStdString(to_string(udp_port)));
    ui->tcp_port->setText(QString::fromStdString(to_string(tcp_port)));
}
