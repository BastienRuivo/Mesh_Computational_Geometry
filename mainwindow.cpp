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


void MainWindow::on_Wireframe_toggled(bool checked)
{
    ui->widget->_wireframe = checked;
}


void MainWindow::on_Axis_toggled(bool checked)
{
    ui->widget->_axis = checked;
}


void MainWindow::on_Pyramid_clicked()
{
    ui->widget->_geomWorld._mesh.initPyramide();
}


void MainWindow::on_Queen_clicked()
{
    ui->widget->_geomWorld._mesh.loadFromOff("queen.off");
}


void MainWindow::on_Cube_clicked()
{
    ui->widget->_geomWorld._mesh.loadFromOff("kube.off");
}

