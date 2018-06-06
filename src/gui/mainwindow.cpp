#include <iostream>
#include <string>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    server_(new MovieLayer())
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_edit_host_address_textChanged(const QString& /* arg1 */)
{

}

void MainWindow::on_btn_choose_file_clicked(bool /* checked */)
{
    string filePath = QFileDialog::getOpenFileName(this, "Add movie", QString(), tr("Movies (*.avi *.mp4)")).toStdString();
    std::map<string, string>::iterator movie = server_.get_movie_layer()->add_movie(filePath).first;


    ui->list_movies->addItem(QString::fromStdString(movie->first));
}

void MainWindow::on_btn_close_server_clicked()
{
    server_.get_network_layer().reset();

    ui->btn_choose_file->setEnabled(true);
    ui->btn_delete_movie->setEnabled(true);
    ui->edit_max_num_of_clients->setEnabled(true);

    ui->udp_port->clear();
    ui->tcp_port->clear();

    ui->list_peers->clear();

    ui->btn_open_server->setEnabled(true);
    ui->btn_close_server->setEnabled(false);
}

void MainWindow::on_btn_choose_file_clicked()
{

}

void MainWindow::on_btn_delete_movie_clicked()
{
    QList<QListWidgetItem*> selectedItems = ui->list_movies->selectedItems();

    for(int i = 0; i < selectedItems.size(); ++i) {
        QString movie_name = selectedItems[i]->text();

        server_.get_movie_layer()->delete_movie(movie_name.toStdString());
    }

    qDeleteAll(selectedItems);
}

void MainWindow::on_btn_kick_peer_clicked()
{

}

void MainWindow::on_btn_show_details_clicked()
{

}

void MainWindow::user_connected(const std::string& ip_with_tcp_port) {
    ui->list_peers->addItem(QString::fromStdString(ip_with_tcp_port));
}

void MainWindow::user_disconnected(const std::string& ip_with_tcp_port) {
    auto list = ui->list_peers->findItems(QString::fromStdString(ip_with_tcp_port), Qt::MatchExactly);
    list.at(0)->~QListWidgetItem();
}


void MainWindow::on_btn_open_server_clicked()
{
    // No adding or deleting movies when network is up
    ui->btn_choose_file->setEnabled(false);
    ui->btn_delete_movie->setEnabled(false);

    ui->btn_open_server->setEnabled(false);
    ui->btn_close_server->setEnabled(true);
    ui->edit_max_num_of_clients->setEnabled(false);
    std::string host_address = ui->edit_host_address->text().toStdString();

    unsigned short max_num_of_clients = 1;
    try {
        max_num_of_clients = std::stoi(ui->edit_max_num_of_clients->text().toStdString());
    } catch (std::exception& err) {
        std::cerr << "Error during 'max number of clients' field conversion" << std::endl;
    }
    server_.init_network_layer(max_num_of_clients);

    qRegisterMetaType<std::string>("std::string");

    /*
     * Connect network_layer to peer list
     */
    QObject::connect(&(*server_.get_network_layer()), &network_layer::user_connects,
                     this, &MainWindow::user_connected);
    QObject::connect(&(*server_.get_network_layer()), &network_layer::user_disconnects,
                     this, &MainWindow::user_disconnected);


    unsigned short udp_port = server_.get_udp_port();
    unsigned short tcp_port = server_.get_tcp_port();
    ui->udp_port->setText(QString::fromStdString(to_string(udp_port)));
    ui->tcp_port->setText(QString::fromStdString(to_string(tcp_port)));
}
