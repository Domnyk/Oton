#include <iostream>
#include <string>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    server_(new movie_layer())
{
    ui->setupUi(this);
    server_.get_movie_layer()->add_new_movie("/Users/dominiktrusinski/Programowanie/C++/Oton/data/Megamind.avi");
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
    // server.reset(nullptr);

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

void MainWindow::non_list_peers_user_connects(const std::string& ip) {
    std::cout << "IP is: " << ip << std::endl;

    ui->list_peers->addItem(QString::fromStdString(ip));
    ui->list_peers->update();
}

void MainWindow::on_btn_open_server_clicked()
{
    ui->btn_open_server->setEnabled(false);
    ui->btn_close_server->setEnabled(true);
    std::string host_address = ui->edit_host_address->text().toStdString();

    server_.init_network_layer();

    qRegisterMetaType<std::string>("std::string");

    /*
     * Connect network_layer to peer list
     */
    QObject::connect(&(*server_.get_network_layer()), &network_layer::user_connects,
                     this, &MainWindow::non_list_peers_user_connects);


    unsigned short udp_port = server_.get_udp_port();
    unsigned short tcp_port = server_.get_tcp_port();
    ui->udp_port->setText(QString::fromStdString(to_string(udp_port)));
    ui->tcp_port->setText(QString::fromStdString(to_string(tcp_port)));
}
