#include <iostream>
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
    std::string host_address = ui->edit_host_address->text().toStdString();
    unsigned short port = ui->edit_port->text().toUShort();

    Server server(host_address, port);
}
