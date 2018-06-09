#include <iostream>
#include <string>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Movie.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    server_(new MovieList())
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
    string movie_filepath = QFileDialog::getOpenFileName(this, "Add movie", QString(), tr("Movies (*.avi *.mp4)")).toStdString();
    bool movie_added = server_.get_movie_layer()->add_movie(movie_filepath);

    if(!movie_added) {
        ui->status_value_label->setText(QString::fromStdString("Movie already on list!"));
        return;
    }

    std::string movie_filename = Movie::get_filename(movie_filepath);
    ui->list_movies->addItem(QString::fromStdString(movie_filename));
    ui->status_value_label->setText(QString::fromStdString("Movie added"));
}

void MainWindow::on_btn_close_server_clicked()
{
    emit server_closes();

    server_.get_network_layer().reset(nullptr);

    ui->btn_choose_file->setEnabled(true);
    ui->btn_delete_movie->setEnabled(true);
    ui->edit_max_num_of_clients->setEnabled(true);

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
        ui->status_value_label->setText(QString::fromStdString("Movie deleted"));
    }

    qDeleteAll(selectedItems);
}

void MainWindow::user_connected(const std::string& ip_with_tcp_port) {
    ui->list_peers->addItem(QString::fromStdString(ip_with_tcp_port));
}

void MainWindow::user_disconnected(const std::string& ip_with_tcp_port) {
    auto list = ui->list_peers->findItems(QString::fromStdString(ip_with_tcp_port), Qt::MatchExactly);

    if(list.empty()) {
        return;
    }

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

    unsigned short max_num_of_clients = 1;
    try {
        max_num_of_clients = std::stoi(ui->edit_max_num_of_clients->text().toStdString());
    } catch (std::exception& err) {
        std::cerr << "Error during 'max number of clients' field conversion" << std::endl;
    }
    server_.init_network_layer(max_num_of_clients);
    ui->edit_max_num_of_clients->setText(QString::fromStdString(std::to_string(max_num_of_clients)));

    qRegisterMetaType<std::string>("std::string");

    /*
     * Connect network_layer to peer list
     */
    QObject::connect(&(*server_.get_network_layer()), &NetworkLayer::user_connects,
                     this, &MainWindow::user_connected);
    QObject::connect(&(*server_.get_network_layer()), &NetworkLayer::user_disconnects,
                     this, &MainWindow::user_disconnected);

    QObject::connect(this, &MainWindow::server_closes, &(*server_.get_network_layer()), &NetworkLayer::server_closes);

    unsigned short tcp_port = server_.get_tcp_port();
    ui->tcp_port->setText(QString::fromStdString(to_string(tcp_port)));
}
