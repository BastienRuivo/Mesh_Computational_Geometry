#ifndef MESH_H
#define MESH_H

#include <QOpenGLWidget>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
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
    std::vector<int> vertexEnter;
    std::vector<int> adjacents;

    std::vector<int> triangles;
    std::vector<double> colors;

    std::vector<bool> visited;

    std::string _name;

public:
    Mesh(); // Constructors automatically called to initialize a Mesh (default strategy)
    void clear();

    void initTetrahedron();
    void initPyramide();
    void initBBox();

    void saveToOffFile(const std::string & name);
    void saveToOffFile();
    void loadFromOff(const std::string & path);

    void paintAdjacents();

    ~Mesh(); // Destructor automatically called before a Mesh is destroyed (default strategy)
    void drawMesh(bool useVisited);
    void drawMeshWireframe(bool useVisited);

    void visit(int i, int t);
    void visitAll(int i);

    std::string getName();
};

class GeometricWorld //Generally used to create a singleton instance
{
  QVector<Point> _bBox;  // Bounding box // ou std::vector
public :
  GeometricWorld();
  void drawAxis();
  void drawWorld(bool wireframed, bool visited);
  // ** TP Can be extended with further elements;
  Mesh _mesh;
};


#endif // MESH_H
