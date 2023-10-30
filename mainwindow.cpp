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

//        for(auto itf = it.beginCirculatorFacesIterator(); itf != it.endCirculatorFacesIterator(); ++itf) {
//            int sommet = *it;
//            int face = *itf;
//        }
    }

//    for(auto it = m.beginFacesIterator(); it != m.endFacesIterator(); ++it) {
//        int v = *it;
//        std::cout << "F :: " << v / 3 <<  " :: V :: " << m.triangles[v] << " " << m.triangles[v+1] << " " << m.triangles[v+2] << " :: A :: " << m.adjacents[v] << " " << m.adjacents[v+1] << " " << m.adjacents[v+2] << std::endl;
//    }



    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-now;
    std::cout << "elapsed time: " << elapsed_seconds.count() * 1000 << "ms for " << m.triangles.size() << " tri" << std::endl;
}



void MainWindow::on_Custom_clicked()
{
    auto & m = ui->widget->_geomWorld._mesh;
    //QUEEN m.flipEdge(0, 6516);
    //m.flipEdge(0, 1);

    std::vector<vec3> points = {
        vec3(1, -1, 0),
        vec3(-1, -0.5, 0)
    };

    for(int i = 0; i < 100; i++) {
        vec3 p(MathHelper::randFloat(-1, 2), MathHelper::randFloat(-1, 2), 0);
        m.naiveInsertion(p);

    }


//    vec3 p = points[ah];
//    ah = (ah + 1) % points.size();
}

void MainWindow::on_Cube_2_clicked()
{
    MeshHelper::loadFromOff(ui->widget->_geomWorld._mesh, "Patron.off");
}


void MainWindow::on_Colors_clicked()
{
    ui->widget->param.toggleColors();
}

