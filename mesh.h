#ifndef MESH_H
#define MESH_H

#include <QOpenGLWidget>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "parameters.h"
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
public:
    std::vector<Point> vertices;
    std::vector<int> vertexEnter;
    std::vector<int> adjacents;

    std::vector<int> triangles;
    std::vector<double> colors;

    std::vector<bool> visited;
    Mesh(); // Constructors automatically called to initialize a Mesh (default strategy)
    void clear();

    void paintAdjacents();

    ~Mesh(); // Destructor automatically called before a Mesh is destroyed (default strategy)
    void draw(GLuint primitive);

    void visit(int i, int t);
    void visitAll(int i);

    class CirculatorFacesIterator
    {
        Mesh * M;
        int vertex;
        int face;
        int firstFace;
        int previousFace;
    public:

        CirculatorFacesIterator(Mesh * m, int vertex, int faceIndex = -1, int firstFace = -1);
        bool operator!=(const CirculatorFacesIterator& other) const;
        CirculatorFacesIterator& operator++();
        int operator*() const;
    };

    CirculatorFacesIterator begin(int vertex);
    CirculatorFacesIterator end(int vertex);

    class VertexIterator {
    private:
        int current;
        Mesh * m;
    public:
        VertexIterator(Mesh * M, int current);
        bool operator!=(const VertexIterator& other) const;
        VertexIterator& operator++();
        int operator*() const;

        CirculatorFacesIterator beginFaceIterator();
        CirculatorFacesIterator endFaceIterator();
    };

    VertexIterator begin();
    VertexIterator end();

    class FacesIterator {
    private:
        int current;
        Mesh * m;
    public:
        FacesIterator(Mesh * M, int current);
        bool operator!=(const FacesIterator& other) const;
        FacesIterator& operator++();
        int operator*() const;

        CirculatorFacesIterator beginFaceIterator();
        CirculatorFacesIterator endFaceIterator();
    };

};

class GeometricWorld //Generally used to create a singleton instance
{
  QVector<Point> _bBox;  // Bounding box // ou std::vector
public :
  GeometricWorld();
  void drawAxis();
  void drawWorld(const Parameters & param);
  // ** TP Can be extended with further elements;
  Mesh _mesh;
};


#endif // MESH_H
