#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <chrono>

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
    auto now = std::chrono::system_clock::now();
    auto & m = ui->widget->_geomWorld._mesh;

    for (auto it = m.beginVertexIterator(); it != m.endVertexIterator(); ++it) {
        int v = *it;

        vec3 l = m.laplacian(v);

        m.colors[3*v] =  l.x;
        m.colors[3*v+1] = l.y;
        m.colors[3*v+2] = l.z;
    }



    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-now;
    std::cout << "elapsed time: " << elapsed_seconds.count() * 1000 << "ms for " << m.triangles.size() << " tri" << std::endl;
}

void MainWindow::on_Custom_clicked()
{
    auto & m = ui->widget->_geomWorld._mesh;
    // QUEEN m.flipEdge(0, 6516);
    //m.flipEdge(0, 1);
    vec3 p = vec3(rand() / (double)RAND_MAX, 0, rand() / (double)RAND_MAX);
    m.naiveInsertion(p);
}


void MainWindow::on_Cube_2_clicked()
{
    MeshHelper::loadFromOff(ui->widget->_geomWorld._mesh, "Patron.off");
}

