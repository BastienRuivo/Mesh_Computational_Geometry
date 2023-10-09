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
#include "mathhelper.h"
// TO MODIFY



//** TP : TO MODIFY

class Mesh
{
public:
    std::vector<vec3> vertices;
    std::vector<int> vertexEnter;
    std::vector<int> adjacents;

    std::vector<int> triangles;
    std::vector<double> colors;

    int infIndex = -1;

    std::vector<bool> visited;
    Mesh(); // Constructors automatically called to initialize a Mesh (default strategy)
    void clear();

    void paintAdjacents();

    ~Mesh(); // Destructor automatically called before a Mesh is destroyed (default strategy)
    void draw(GLuint primitive);

    void visit(int i, int t);
    void visitAll(int i);

    int localIndex(int vertex, int face) {
        for (int var = 0; var < 3; ++var) {
            if(triangles[face*3 + var] == vertex) {
                return var;
            }
        }
        return -1;
    }

    int findOppositePoint(int t0, int t1) {
        for (int v = 0; v < 3; ++v) {
            if(adjacents[t0 * 3 + v] == t1) {
                return v;
            }
        }
        return -1;
    }

    int thirdIndex(int vi, int vj, int face) {
        int li = localIndex(vi, face);
        int lj = localIndex(vj, face);
        return 3 - (li + lj);
    }

    // take a vertex and a function as parameter and return the laplacian of the vertex
    vec3 laplacian(int vi);

    void addPointToTriangle(const vec3 & p, int triangleIndex);
    void addPoint(const vec3 & p);
    int findTriangle(const vec3 & p);

    void flipEdge(int v1, int v2);

    void naiveInsertion(const vec3 & p) {
        int f = findTriangle(p);
        if(f == -1) return;
        addPointToTriangle(p, f);
    }

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

    CirculatorFacesIterator beginCirculatorFacesIterator(int vertex);
    CirculatorFacesIterator endCirculatorFacesIterator(int vertex);

    class CirculatorVertexIterator
    {
        Mesh * M;
        int vertex;
        int face;
        int firstFace;
        int previousFace;
    public:

        CirculatorVertexIterator(Mesh * m, int vertex, int faceIndex = -1, int firstFace = -1);
        bool operator!=(const CirculatorVertexIterator& other) const;
        CirculatorVertexIterator& operator++();
        int operator*() const;
        int getFace() const {
            return face;
        }
    };

    CirculatorVertexIterator beginCirculatorVertexIterator(int vertex);
    CirculatorVertexIterator endCirculatorVertexIterator(int vertex);

    class VertexIterator {
    private:
        int current;
        Mesh * m;
    public:
        VertexIterator(Mesh * M, int current);
        bool operator!=(const VertexIterator& other) const;
        VertexIterator& operator++();
        int operator*() const;

        CirculatorFacesIterator beginCirculatorFacesIterator();
        CirculatorFacesIterator endCirculatorFacesIterator();

        CirculatorVertexIterator beginCirculatorVertexIterator();
        CirculatorVertexIterator endCirculatorVertexIterator();
    };

    VertexIterator beginVertexIterator();
    VertexIterator endVertexIterator();

    class FacesIterator {
    private:
        int current;
        Mesh * m;
    public:
        FacesIterator(Mesh * M, int current);
        bool operator!=(const FacesIterator& other) const;
        FacesIterator& operator++();
        int operator*() const;
    };

    FacesIterator beginFacesIterator();
    FacesIterator endFacesIterator();
};

class GeometricWorld //Generally used to create a singleton instance
{
    QVector<vec3> _bBox;  // Bounding box // ou std::vector
public :
  GeometricWorld();
  void drawAxis();
  void drawWorld(const Parameters & param);
  // ** TP Can be extended with further elements;
  Mesh _mesh;
};


#endif // MESH_H
