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
    void drawMesh(bool useVisited);
    void drawMeshWireframe(bool useVisited);

    void visit(int i, int t);
    void visitAll(int i);

    class VertexFacesIterator
    {
        Mesh * M;
        int vertex;
        int face;
        int firstFace;
        int previousFace;
    public:

        VertexFacesIterator(Mesh * m, int vertex, int faceIndex = -1, int firstFace = -1);
        bool operator!=(const VertexFacesIterator& other) const;
        VertexFacesIterator& operator++();
        int operator*() const;
    };

    VertexFacesIterator begin(int vertex);

    VertexFacesIterator end(int vertex);

    class VertexIterator {
    private:
        int current;
        Mesh * m;
    public:
        VertexIterator(Mesh * M, int current);
        bool operator!=(const VertexIterator& other) const;
        VertexIterator& operator++();
        int operator*() const;
        Point& operator*();

        VertexFacesIterator beginFaceIterator();
        VertexFacesIterator endFaceIterator();
    };

    VertexIterator begin();
    VertexIterator end();
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
