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
    std::vector<double> colorsTri = {
        0.0, 0.0, 0.0, // black 0
        0.0, 0.0, 1.0, // blue 1
        0.0, 1.0, 0.0, // green 2
        0.0, 1.0, 1.0, // cyan 3
        1.0, 0.0, 0.0, // red 4
        1.0, 0.0, 1.0, // magenta 5
        1.0, 1.0, 0.0, // yellow 6
        1.0, 1.0, 1.0, // white 7
        0.5, 0.5, 0.5,  // gray 8
        1.0, 0.5, 0.0, // orange 9
        0.5, 0.0, 0.5, // purple 10
    };

    vec3 aaBox[2]; // Axis aligned box (bounding box)

    Mesh(); // Constructors automatically called to initialize a Mesh (default strategy)
    void clear();

    void paintAdjacents();

    ~Mesh(); // Destructor automatically called before a Mesh is destroyed (default strategy)
    void draw(bool colors);

    int localIndex(int vertex, int face) {
        for (int var = 0; var < 3; ++var) {
            if(triangles[face*3 + var] == vertex) {
                return var;
            }
        }
        return -1;
    }

    int findOppositePoint(int faceToSearch, int faceToFind) {
        for (int v = 0; v < 3; ++v) {
            if(adjacents[faceToSearch * 3 + v] == faceToFind) {
                return v;
            }
        }
        std::cout << "ERROR CAN'T FIND OPPOSITE:: " << faceToSearch << " :: " << faceToFind << std::endl;
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
    void findClosestTriangle(const vec3 & p);
    void addPoint(const vec3 & p);
    int findTriangle(const vec3 & p);

    void flipEdge(int v1, int v2);

    void addFirstIntoConvexHull(const vec3 & p, int face) {
        int pid = vertices.size();
        vertices.push_back(p);

        colors.push_back(MathHelper::randFloat(0, 1));
        colors.push_back(MathHelper::randFloat(0, 1));
        colors.push_back(MathHelper::randFloat(0, 1));

        int v1 = triangles[face * 3 + 1];
        int v2 = triangles[face * 3 + 2];


        int t1 = triangles.size() / 3;
        triangles.push_back(pid);
        triangles.push_back(v1);
        triangles.push_back(v2);

        //std::cout << "t1 = " << t1 << " [" << triangles[t1 * 3] << " " << triangles[t1 * 3 + 1] << " " << triangles[t1 * 3 + 2] << "]" << std::endl;

        vertexEnter[pid] = t1;
        vertexEnter[v2] = t1;
        vertexEnter[v1] = t1;

        int t2 = triangles.size() / 3;
        triangles.push_back(0);
        triangles.push_back(pid);
        triangles.push_back(v2);

        //std::cout << "t2 = " << t2 << " [" << triangles[t2 * 3] << " " << triangles[t2 * 3 + 1] << " " << triangles[t2 * 3 + 2] << "]" << std::endl;

        vertexEnter[0] = t2;

        int opp_v0_old = adjacents[face * 3];
        int opp_v1_old = adjacents[face * 3 + 1];
        int opp_v2_old = adjacents[face * 3 + 2];

        adjacents[opp_v0_old * 3 + findOppositePoint(opp_v0_old, face)] = t1;
        adjacents[opp_v1_old * 3 + findOppositePoint(opp_v1_old, face)] = t2;

        adjacents.push_back(opp_v0_old);
        adjacents.push_back(t2);
        adjacents.push_back(face);

        adjacents.push_back(t1);
        adjacents.push_back(opp_v1_old);
        adjacents.push_back(face);


        triangles[face * 3 + 2] = pid;
        //std::cout << " face = " << face << " [" << triangles[face * 3] << " " << triangles[face * 3 + 1] << " " << triangles[face * 3 + 2] << "]" << std::endl;

        adjacents[face * 3] = t1;
        adjacents[face * 3 + 1] = t2;
        adjacents[face * 3 + 2] = opp_v2_old;

        vertexEnter.push_back(t1);
        vertexEnter[v1] = t1;
        vertexEnter[v2] = t1;
    }

    int opposentFaceContainingVertex(int vertex, int face) {
        for (int i = 0; i < 3; ++i) {
            if(localIndex(vertex, adjacents[face * 3 + i]) != -1) {
                return adjacents[face * 3 + i];
            }
        }
        return -1;
    }

    void insertIntoConvexHull(const vec3 & p) {
        std::vector<int> faces;
        if(vertices.size() == 0) return;
        for(auto it = beginCirculatorFacesIterator(0); it != endCirculatorFacesIterator(0); ++it) {
            int face = *it;
            int localIndex = this->localIndex(0, face);
            int v0 = triangles[face * 3 + (localIndex + 1) % 3];
            int v1 = triangles[face * 3 + (localIndex + 2) % 3];
            int orientation = MathHelper::orientation(vertices[v0], vertices[v1], p);
            if(orientation == -1) {
                faces.push_back(face);
            }
        }
        std::cout << "Number of faces to add = " << faces.size() << std::endl;
        if(faces.size() > 0) {
            addFirstIntoConvexHull(p, faces[0]);
            for(int i = 1; i < faces.size(); i++) {
                int toFlip = opposentFaceContainingVertex(vertices.size() - 1, faces[i]);
                if(toFlip != -1) {
                    flipEdge(faces[i], toFlip);
                } else {
                    std::cout << "Achievement get :: How did we get here ?" << std::endl;
                }
            }
        }
    }

    void naiveInsertion(const vec3 & p) {
        int f = findTriangle(p);
        if(f == -1) {
            std::cout << "Out of convex hull" << std::endl;
            insertIntoConvexHull(p);
        } else {
            std::cout << " Inside mesh " << std::endl;
            addPointToTriangle(p, f);
        }
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
