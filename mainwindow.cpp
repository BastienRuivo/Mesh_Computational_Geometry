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
    ui->widget->param.setWireframe(checked);
}


void MainWindow::on_Axis_toggled(bool checked)
{
    ui->widget->param.setAxis(checked);
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

void MainWindow::on_Circular_clicked()
{
    auto & m = ui->widget->_geomWorld._mesh;

    for (Mesh::VertexIterator it = m.begin(); it != m.end(); ++it) {
        int v = *it;
        std::cout << "Vertex[" << v << "] : [";
        for (Mesh::CirculatorFacesIterator itf = it.beginFaceIterator(); itf != it.endFaceIterator(); ++itf) {
            int f = *itf;
            std::cout << f << " ";
        }
        std::cout << "]" << std::endl;
    }
}

