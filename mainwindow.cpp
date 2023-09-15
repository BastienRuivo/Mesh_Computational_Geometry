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

void MainWindow::on_SaveOff_clicked()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d-%H-%M-%S");
    MeshHelper::saveToOffFile(ui->widget->_geomWorld._mesh, ss.str());
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
    if(!MeshHelper::offExist("Pyramide")) MeshHelper::generatePyramide();
    MeshHelper::loadFromOff(ui->widget->_geomWorld._mesh, "Pyramide.off");
}


void MainWindow::on_Queen_clicked()
{
    MeshHelper::loadFromOff(ui->widget->_geomWorld._mesh, "queen.off");
}


void MainWindow::on_Cube_clicked()
{
    MeshHelper::loadFromOff(ui->widget->_geomWorld._mesh, "cube.off");
}

void MainWindow::on_Tetra_clicked()
{
    if(!MeshHelper::offExist("Tetrahedron")) MeshHelper::generateTetrahedron();
    MeshHelper::loadFromOff(ui->widget->_geomWorld._mesh, "Tetrahedron.off");
}

void MainWindow::on_Wireframe_2_toggled(bool checked)
{
    ui->widget->_visited = checked;
}


void MainWindow::on_CircularValue_valueChanged(int v)
{
    ui->widget->_circularValue = v;
}


void MainWindow::on_Circular_clicked()
{
    auto & mesh = ui->widget->_geomWorld._mesh;

    int v = ui->widget->_circularValue;
    std::cout << "iterating on " << v << std::endl;

    for(Mesh::VertexFacesIterator it = mesh.begin(v); it != mesh.end(v); ++it) {
        int face = *it;
        std::cout << "Face : " << face << std::endl;
    }
}

