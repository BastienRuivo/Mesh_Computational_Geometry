#include "mesh.h"

GeometricWorld::GeometricWorld()
{
    double width=0.5, depth=0.6, height=0.8;
    _bBox.push_back(vec3(-0.5*width,-0.5*depth,-0.5*height)); //0
    _bBox.push_back(vec3(-0.5*width,0.5*depth,-0.5*height)); // 1
    _bBox.push_back(vec3(0.5*width,-0.5*depth,-0.5*height)); // 2
    _bBox.push_back(vec3(-0.5*width,-0.5*depth,0.5*height)); // 3
}

// The following functions could be displaced into a module OpenGLDisplayGeometricWorld that would include mesh.h

// Draw a Point
void glPointDraw(const vec3 & p) {
    glVertex3f(p.x, p.y, p.z);
}

//Example with a bBox
void GeometricWorld::drawWorld(const Parameters & param) {
    if(param.wireframe()) {
        _mesh.draw(GL_LINE_STRIP);
    } else {
        _mesh.draw(GL_TRIANGLES);
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

vec3 Mesh::laplacian(int vi) {
    vec3 sum(0, 0, 0);
    float area = 0;
    for(auto it = beginCirculatorVertexIterator(vi); it != endCirculatorVertexIterator(vi); ++it) {
        // compute (cot(aij) + cot(bij)) * 0.5
        int vj = *it;
        int fj = it.getFace();

        if(fj == -1) break;


        int vj_op = triangles[fj*3 + thirdIndex(vi, vj, fj)];

        // compute vn which is in the opposite face of vj

        int fn = adjacents[fj*3 + thirdIndex(vi, vj, fj)];


        int vn = triangles[fn*3 + thirdIndex(vi, vj, fn)];

        //int vn_op = triangles[fn*3 + thirdIndex(vi, vn, fn)];


        // compute cotangent

        vec3 pi = vertices[vi];
        vec3 pj = vertices[vj];
        vec3 pn = vertices[vn];
        vec3 pop = vertices[vj_op];

        // cotangent

        float cot0 = (pop - pi).norm() / (pj - pi).norm();
        float cot1 = (pn - pi).norm() / (pj - pi).norm();

        sum += ((pj - pi) * (cot0 + cot1)) * 0.5;
    }

    return sum.normalize();
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

//    for (int var = 0; var < adjacents.size(); var+=3) {
//        std::cout << "for triangle [" << triangles[var] << " " << triangles[var+1] << " " << triangles[var+2] << "] :: [" << adjacents[var] << " " << adjacents[var+1] << " " << adjacents[var+2] << "]" << std::endl;
//    }

    std::cout << "Number of edges : " << map.size() << std::endl;

    visited.resize(triangles.size()/3, false);
}

Mesh::~Mesh() {
    // Destructor automatically called before a Mesh is destroyed (default strategy)
}

void Mesh::draw(GLuint primitive) {
    for (size_t i = 0; i < triangles.size(); i+=3)
    {
        glBegin(primitive);

        glColor3f(colors[triangles[i] * 3], colors[triangles[i] * 3 + 1], colors[triangles[i] * 3 + 2]);
        glPointDraw(vertices[triangles[i]]);
        glColor3f(colors[triangles[i+1] * 3], colors[triangles[i+1] * 3 + 1], colors[triangles[i+1] * 3 + 2]);
        glPointDraw(vertices[triangles[i+1]]);
        glColor3f(colors[triangles[i+2] * 3], colors[triangles[i+2] * 3 + 1], colors[triangles[i+2] * 3 + 2]);
        glPointDraw(vertices[triangles[i+2]]);
        glEnd();
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

Mesh::CirculatorFacesIterator::CirculatorFacesIterator(Mesh * m, int vertex, int faceIndex, int firstFace) {
    M = m;
    this->vertex = vertex;
    this->face = faceIndex;
    this->firstFace = firstFace;
}

bool Mesh::CirculatorFacesIterator::operator!=(const CirculatorFacesIterator& other) const {
    return M != other.M || vertex != other.vertex || face != other.face || firstFace != other.firstFace;
}

Mesh::CirculatorFacesIterator& Mesh::CirculatorFacesIterator::operator++() {
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

    face = M->adjacents[face * 3 + ((localVertexIndex + 2) % 3)];

    return *this;
}

int Mesh::CirculatorFacesIterator::operator*() const  {
    return face;
}

Mesh::CirculatorFacesIterator Mesh::beginCirculatorFacesIterator(int vertex) {
    return CirculatorFacesIterator(this, vertex, this->vertexEnter[vertex], -1);
}

Mesh::CirculatorFacesIterator Mesh::endCirculatorFacesIterator(int vertex) {
    return CirculatorFacesIterator(this, vertex, this->vertexEnter[vertex], this->vertexEnter[vertex]);
}

Mesh::CirculatorVertexIterator::CirculatorVertexIterator(Mesh * m, int vertex, int faceIndex, int firstFace) {
    M = m;
    this->vertex = vertex;
    this->face = faceIndex;
    this->firstFace = firstFace;
}

bool Mesh::CirculatorVertexIterator::operator!=(const CirculatorVertexIterator& other) const {
    return M != other.M || vertex != other.vertex || face != other.face || firstFace != other.firstFace;
}

Mesh::CirculatorVertexIterator& Mesh::CirculatorVertexIterator::operator++() {
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

    face = M->adjacents[face * 3 + ((localVertexIndex + 2) % 3)];
    return *this;
}

int Mesh::CirculatorVertexIterator::operator*() const  {
    int localVertexIndex = -1;
    for(int i = 0; i < 3; i++) {
        if(M->triangles[face*3 + i] == vertex) {
            localVertexIndex = i;
            break;
        }
    }
    return M->triangles[face*3 + ((localVertexIndex + 1) % 3)];
}

Mesh::CirculatorVertexIterator Mesh::beginCirculatorVertexIterator(int vertex) {
    return CirculatorVertexIterator(this, vertex, this->vertexEnter[vertex], -1);
}

Mesh::CirculatorVertexIterator Mesh::endCirculatorVertexIterator(int vertex) {
    return CirculatorVertexIterator(this, vertex, this->vertexEnter[vertex], this->vertexEnter[vertex]);
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

Mesh::CirculatorFacesIterator Mesh::VertexIterator::beginCirculatorFacesIterator() {
    return m->beginCirculatorFacesIterator(this->current);
}

Mesh::CirculatorFacesIterator Mesh::VertexIterator::endCirculatorFacesIterator() {
    return m->endCirculatorFacesIterator(this->current);
}

Mesh::CirculatorVertexIterator Mesh::VertexIterator::beginCirculatorVertexIterator() {
    return m->beginCirculatorVertexIterator(this->current);
}

Mesh::CirculatorVertexIterator Mesh::VertexIterator::endCirculatorVertexIterator() {
    return m->endCirculatorVertexIterator(this->current);
}

Mesh::VertexIterator Mesh::beginVertexIterator() {
    return VertexIterator(this, 0);
}

Mesh::VertexIterator Mesh::endVertexIterator() {
    return VertexIterator(this, vertices.size());
}

Mesh::FacesIterator::FacesIterator(Mesh * m, int current) {
    this->current = current;
    this->m = m;
}

bool Mesh::FacesIterator::operator!=(const Mesh::FacesIterator & other) const {
    return other.current != this->current;
}

Mesh::FacesIterator& Mesh::FacesIterator::operator++() {
    this->current+=3;
    return *this;
}

int Mesh::FacesIterator::operator*() const {
    return this->current;
}

Mesh::FacesIterator Mesh::beginFacesIterator() {
    return Mesh::FacesIterator(this, 0);
}

Mesh::FacesIterator Mesh::endFacesIterator() {
    return Mesh::FacesIterator(this, triangles.size());
}

int Mesh::findTriangle(const vec3 & p) {
    for(int i = 0; i < triangles.size(); i+=3) {
        vec3 p1 = vertices[triangles[i]];
        vec3 p2 = vertices[triangles[i+1]];
        vec3 p3 = vertices[triangles[i+2]];

        if(MathHelper::isInTriangle(p, p1, p2, p3)) {
            return i;
        }
    }
    return -1;
}

void Mesh::addPointToTriangle(const vec3 & p, int triangleIndex) {
    this->vertices.push_back(p);
    this->vertexEnter.push_back(triangleIndex);

    int v0 = triangles[triangleIndex*3];
    int v1 = triangles[triangleIndex*3+1];
    int v2 = triangles[triangleIndex*3+2];
    int n = this->vertices.size() - 1;

    triangles[triangleIndex*3] = vertices.size() - 1;

    triangles.push_back(n);
    triangles.push_back(v0);
    triangles.push_back(v1);

    triangles.push_back(n);
    triangles.push_back(v2);
    triangles.push_back(v0);

    // adjencies

    int a0 = adjacents[triangleIndex*3];
    int a1 = adjacents[triangleIndex*3+1];
    int a2 = adjacents[triangleIndex*3+2];

    int tn0 = triangles.size() / 3 - 2;
    int tn1 = triangles.size() / 3 - 1;

    adjacents[triangleIndex*3 + 1] = tn1;
    adjacents[triangleIndex*3 + 2] = tn0;

    adjacents.push_back(a2);
    adjacents.push_back(triangleIndex);
    adjacents.push_back(tn1);

    adjacents.push_back(a1);
    adjacents.push_back(tn0);
    adjacents.push_back(triangleIndex);

}

