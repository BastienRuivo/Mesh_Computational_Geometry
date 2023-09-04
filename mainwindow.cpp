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

void MainWindow::on_Tetra_clicked()
{
    ui->widget->_geomWorld._mesh.initTetrahedron();
}


void MainWindow::on_SaveOff_clicked()
{
    ui->widget->_geomWorld._mesh.saveToOffFile();
}

