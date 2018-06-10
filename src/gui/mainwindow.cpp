#include <iostream>
#include <string>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Movie.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    // server_(new MovieList())
{
    ui->setupUi(this);
    ui->edit_max_num_of_clients->setValidator(new QIntValidator(0, 100, this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_edit_host_address_textChanged(const QString& /* arg1 */)
{

}

void MainWindow::handle_movie_already_on_list() {
    ui->status_value_label->setText(QString::fromStdString("Movie already on list!"));
}

void MainWindow::handle_first_movie_added(const QString movie_name) {
    ui->btn_delete_movie->setEnabled(true);
    handle_movie_added(movie_name);
}

void MainWindow::handle_movie_added(const QString movie_name) {
    ui->list_movies->addItem(movie_name);
    ui->status_value_label->setText(QString::fromStdString("Movie added"));
}

void MainWindow::on_btn_choose_file_clicked(bool /* checked */)
{
    QString movie_filepath = QFileDialog::getOpenFileName(this, "Add movie", QString(), tr("Movies (*.avi *.mp4)"));
    if (movie_filepath.isEmpty()) {
        ui->status_value_label->setText("Waiting for action");
        return;
    }

    emit file_has_been_choosed(movie_filepath);
}

void MainWindow::on_btn_close_server_clicked()
{
    emit stop_distributing();

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

        emit delete_has_been_clicked(movie_name);
        ui->status_value_label->setText(QString::fromStdString("Movie deleted"));
    }
    qDeleteAll(selectedItems);
}

void MainWindow::handle_last_movie_deleted() {
    ui->btn_delete_movie->setEnabled(false);
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
    emit start_distributing(max_num_of_clients);

    ui->edit_max_num_of_clients->setText(QString::fromStdString(std::to_string(max_num_of_clients)));



    /*
     * Connect network_layer to peer list
     */
    /* QObject::connect(&(*server_.get_network_layer()), &NetworkLayer::user_connects,
                     this, &MainWindow::user_connected);
    QObject::connect(&(*server_.get_network_layer()), &NetworkLayer::user_disconnects,
                     this, &MainWindow::user_disconnected);

    QObject::connect(this, &MainWindow::server_closes, &(*server_.get_network_layer()), &NetworkLayer::server_closes);

    ui->tcp_port->setText(QString::fromStdString(to_string(tcp_port))); */
}

void MainWindow::handle_server_distributes(unsigned short port_num) {
    ui->tcp_port->setText(QString::number(port_num));
}
