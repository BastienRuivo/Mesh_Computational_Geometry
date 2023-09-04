#ifndef MESH_H
#define MESH_H

#include <QOpenGLWidget>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
// TO MODIFY
class Point
{
public:
    double _x;
    double _y;
    double _z;

    Point():_x(),_y(),_z() {}
    Point(float x_, float y_, float z_):_x(x_),_y(y_),_z(z_) {}
};


//** TP : TO MODIFY

class Mesh
{
  std::vector<Point> vertices;
  std::vector<int> triangles;
  std::vector<int> neighbours;
  std::vector<Point> colors;

  std::string _name;

public:
    Mesh(); // Constructors automatically called to initialize a Mesh (default strategy)
    void initTetrahedron();
    void initPyramide();
    void initBBox();
    void saveToOffFile(const std::string & name);
    void saveToOffFile();
    ~Mesh(); // Destructor automatically called before a Mesh is destroyed (default strategy)
    void drawMesh();
    void drawMeshWireFrame();
    std::string getName();
};

class GeometricWorld //Generally used to create a singleton instance
{
  QVector<Point> _bBox;  // Bounding box // ou std::vector
public :
  GeometricWorld();
  void draw();
  void drawWireFrame();
  // ** TP Can be extended with further elements;
  Mesh _mesh;
};


#endif // MESH_H
