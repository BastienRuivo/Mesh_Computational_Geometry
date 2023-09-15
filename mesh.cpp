#include "mesh.h"

GeometricWorld::GeometricWorld()
{
    double width=0.5, depth=0.6, height=0.8;
    _bBox.push_back(Point(-0.5*width,-0.5*depth,-0.5*height)); //0
    _bBox.push_back(Point(-0.5*width,0.5*depth,-0.5*height)); // 1
    _bBox.push_back(Point(0.5*width,-0.5*depth,-0.5*height)); // 2
    _bBox.push_back(Point(-0.5*width,-0.5*depth,0.5*height)); // 3
}

// The following functions could be displaced into a module OpenGLDisplayGeometricWorld that would include mesh.h

// Draw a Point
void glPointDraw(const Point & p) {
    glVertex3f(p._x, p._y, p._z);
}

//Example with a bBox
void GeometricWorld::drawWorld(bool wireframed, bool visited) {
    if(wireframed) {
        _mesh.drawMeshWireframe(visited);
    } else {
        _mesh.drawMesh(visited);
    }
}

//Example with a wireframe bBox
void GeometricWorld::drawAxis() {
    glColor3d(0,1,0);
    glBegin(GL_LINE_STRIP);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[1]);
    glEnd();
    glColor3d(0,0,1);
    glBegin(GL_LINE_STRIP);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[2]);
    glEnd();
    glColor3d(1,0,0);
    glBegin(GL_LINE_STRIP);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[3]);
    glEnd();
}

Mesh::Mesh() {
}

void Mesh::clear() {
    vertices.clear();
    vertexEnter.clear();

    triangles.clear();
    colors.clear();

    adjacents.clear();
    visited.clear();
}

std::pair<int, int> makeEdge(int ti, int tj) {
    if(ti < tj) {
        return std::make_pair(ti, tj);
    } else {
        return std::make_pair(tj, ti);
    }
}

void Mesh::paintAdjacents() {
    adjacents.clear();
    vertexEnter.clear();
    // fill the adjacents map wich contains the adjacent triangles of each triangle

    std::map<std::pair<int, int>, std::pair<int, int>> map;

    vertexEnter.resize(vertices.size());

    adjacents.resize(triangles.size(), -1);

    for(size_t t = 0; t < triangles.size(); t+=3) {
        // parcours ses edges
        std::array<std::pair<int, int>, 3> edges;

        // order this way bc the missing t + e is increasing linearly
        int tId = t/3;
        edges[0] = makeEdge(triangles[t+1], triangles[t+2]); // t + 0
        edges[1] = makeEdge(triangles[t+2], triangles[t]); // t + 1
        edges[2] = makeEdge(triangles[t], triangles[t+1]); // t + 2

        vertexEnter[triangles[t]] = tId;
        vertexEnter[triangles[t+1]] = tId;
        vertexEnter[triangles[t+2]] = tId;

        for(int e = 0; e < 3; e++) {
            if(!map.count(edges[e])) {
                map[edges[e]] = std::make_pair(tId, e);
            } else {
                int target = map[edges[e]].first;
                int offset = map[edges[e]].second;

                adjacents[t + e] = target;
                adjacents[target * 3 + offset] = tId;
            }
        }
    }

    for (int var = 0; var < adjacents.size(); var+=3) {
        std::cout << "for triangle [" << triangles[var] << " " << triangles[var+1] << " " << triangles[var+2] << "] :: [" << adjacents[var] << " " << adjacents[var+1] << " " << adjacents[var+2] << "]" << std::endl;
    }

    std::cout << "Number of edges : " << map.size() << std::endl;

    visited.resize(triangles.size()/3, false);
}

Mesh::~Mesh() {
    // Destructor automatically called before a Mesh is destroyed (default strategy)
}

void Mesh::drawMesh(bool useVisited) {

    for (size_t i = 0; i < triangles.size(); i++)
    {
        if(!useVisited || visited[i/3]) {
            if(i % 3 == 0) {
                glEnd();
                glBegin(GL_TRIANGLES);
                glColor3f(colors[i], colors[i+1], colors[i+2]);
            }
            glPointDraw(vertices[triangles[i]]);
        }
    }
    glEnd();
}

void Mesh::drawMeshWireframe(bool useVisited) {

    for (size_t i = 0; i < triangles.size(); i++)
    {
        if(!useVisited || visited[i/3]) {
            if(i % 3 == 0) {
                glEnd();
                glBegin(GL_LINE_STRIP);
                glColor3f(colors[i], colors[i+1], colors[i+2]);
            }
            glPointDraw(vertices[triangles[i]]);
        }
    }
    glEnd();
}

void Mesh::visitAll(int pointToVisit) {
    int t = vertexEnter[pointToVisit];

    if(visited[t]) return;

    visited[t] = true;
    for(int i = 0; i < 3; i++) {
        int v = adjacents[t*3 + i];
        if(!visited[v] && (triangles[v*3] == pointToVisit || triangles[v*3 + 1] == pointToVisit || triangles[v*3 + 2] == pointToVisit)) {
            visit(pointToVisit, v);
        }
    }
}

void Mesh::visit(int pointToVisit, int triangleToVisit) {
    int t = triangleToVisit;
    if(visited[t]) return;

    visited[t] = true;

    for(int i = 0; i < 3; i++) {
        int v = adjacents[t*3 + i];
        if(!visited[v] && (triangles[v*3] == pointToVisit || triangles[v*3 + 1] == pointToVisit || triangles[v*3 + 2] == pointToVisit)) {
            visit(pointToVisit, v);
        }
    }
}

Mesh::VertexFacesIterator::VertexFacesIterator(Mesh * m, int vertex, int faceIndex, int firstFace) {
    M = m;
    this->vertex = vertex;
    this->face = faceIndex;
    this->firstFace = firstFace;
}

bool Mesh::VertexFacesIterator::operator!=(const VertexFacesIterator& other) const {
    return M != other.M || vertex != other.vertex || face != other.face || firstFace != other.firstFace;
}

Mesh::VertexFacesIterator& Mesh::VertexFacesIterator::operator++() {
    if(firstFace == -1) {
        firstFace = face;
    }

    int localVertexIndex = -1;
    for(int i = 0; i < 3; i++) {
        if(M->triangles[face*3 + i] == vertex) {
            localVertexIndex = i;
            break;
        }
    }

    //std::cout << "for vertex " << vertex << " and face " << face << " localVertexIndex is " << localVertexIndex << std::endl;
    int nFace;
    if(previousFace == -1) {
        previousFace = face;
        nFace = M->adjacents[face * 3 + ((localVertexIndex + 2) % 3)];
    } else {
        nFace = M->adjacents[face * 3 + ((localVertexIndex + 2) % 3)];
        if(nFace == previousFace) {
            nFace = M->adjacents[face * 3 + ((localVertexIndex + 1) % 3)];
        }
        previousFace = face;
    }

    face = nFace;

    return *this;
}

int Mesh::VertexFacesIterator::operator*() const  {
    return face;
}

Mesh::VertexFacesIterator Mesh::begin(int vertex) {
    return VertexFacesIterator(this, vertex, this->vertexEnter[vertex], -1);
}

Mesh::VertexFacesIterator Mesh::end(int vertex) {
    return VertexFacesIterator(this, vertex, this->vertexEnter[vertex], this->vertexEnter[vertex]);
}

Mesh::VertexIterator::VertexIterator(Mesh * m, int current) {
    this->current = current;
    this->m = m;
}

bool Mesh::VertexIterator::operator!=(const Mesh::VertexIterator & other) const {
    return other.current != this->current;
}

Mesh::VertexIterator& Mesh::VertexIterator::operator++() {
    this->current++;
    return *this;
}

int Mesh::VertexIterator::operator*() const {
    return this->current;
}

Point& Mesh::VertexIterator::operator*() {
    return m->vertices[this->current];
}

Mesh::VertexFacesIterator Mesh::VertexIterator::beginFaceIterator() {
    return m->begin(this->current);
}

Mesh::VertexFacesIterator Mesh::VertexIterator::endFaceIterator() {
    return m->end(this->current);
}

Mesh::VertexIterator Mesh::begin() {
    return VertexIterator(this, 0);
}

Mesh::VertexIterator Mesh::end() {
    return VertexIterator(this, vertices.size());
}

