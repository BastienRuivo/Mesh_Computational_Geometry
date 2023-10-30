#include "mesh.h"
#include <queue>
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
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    } else {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
    _mesh.draw(param.colors());
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

    vertexEnter.resize(vertices.size(), -1);

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

    std::vector<std::pair<int, int>> convexe;
    for (int var = 0; var < adjacents.size(); var+=3) {
        for(int lid = 0; lid < 3; lid++) {
            if(adjacents[var+lid] == -1) {
                convexe.push_back(std::make_pair(var/3, lid));
            }
        }
    }

    for(auto & c : convexe) {
        triangles.push_back(0);
        triangles.push_back(triangles[c.first * 3 + (c.second + 2) % 3]);
        triangles.push_back(triangles[c.first * 3 + (c.second + 1) % 3]);
        std::cout << " f = " << (triangles.size() / 3) - 1 << " " << 0 << " " << triangles[c.first * 3 + (c.second + 1) % 3] << " " << triangles[c.first * 3 + (c.second + 2) % 3] << std::endl;
    }

    if(convexe.size() != 0) {
        paintAdjacents();
    }

    std::cout << "Number of edges : " << map.size() << std::endl;
}

Mesh::~Mesh() {
    // Destructor automatically called before a Mesh is destroyed (default strategy)
}

void Mesh::draw(bool col) {
    for (size_t i = 0; i < triangles.size(); i+=3)
    {
        glBegin(GL_TRIANGLES);
        if(!col) glColor3f(colorsTri[i % (colorsTri.size())], colorsTri[(i+1) % (colorsTri.size())], colorsTri[(i+2) % (colorsTri.size())]);

        if(col) glColor3f(colors[triangles[i] * 3], colors[triangles[i] * 3 + 1], colors[triangles[i] * 3 + 2]);
        glPointDraw(vertices[triangles[i]]);

        if(col) glColor3f(colors[triangles[i+1] * 3], colors[triangles[i+1] * 3 + 1], colors[triangles[i+1] * 3 + 2]);
        glPointDraw(vertices[triangles[i+1]]);

        if(col) glColor3f(colors[triangles[i+2] * 3], colors[triangles[i+2] * 3 + 1], colors[triangles[i+2] * 3 + 2]);
        glPointDraw(vertices[triangles[i+2]]);

        glEnd();
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



void Mesh::flipEdge(int t1, int t2) {
    int t1_opposite = -1;
    int t2_opposite = -1;
    int local_op_t1 = -1;
    int local_op_t2 = -1;

    std::cout << "Flipping " << t1 << " " << t2 << std::endl;

    local_op_t1 = findOppositePoint(t1, t2);
    local_op_t2 = findOppositePoint(t2, t1);

    int t1o = triangles[t1 * 3 + (local_op_t1 + 2) % 3];
    int t2o = triangles[t2 * 3 + (local_op_t2 + 2) % 3];

    if(local_op_t1 == -1 || local_op_t2 == -1) {
        std::cout << "Edge not found" << std::endl;
        return;
    }
    int update_t2 = adjacents[t2 * 3 + (local_op_t2 + 1) % 3];
    int update_t1 = adjacents[t1 * 3 + (local_op_t1 + 1) % 3];

    int lupdate_op_t2 = findOppositePoint(update_t2, t2);
    int lupdate_op_t1 = findOppositePoint(update_t1, t1);

    if(lupdate_op_t2 == -1 || lupdate_op_t1 == -1) {
        std::cout << "Edge not found NEIGHBORING" << std::endl;
        return;
    }

    t1_opposite = triangles[t1 * 3 + local_op_t1];
    t2_opposite = triangles[t2 * 3 + local_op_t2];

    local_op_t2 = localIndex(t2_opposite, t2);
    triangles[t1 * 3 + (local_op_t1 + 2) % 3] = t2_opposite;
    triangles[t2 * 3 + (local_op_t2 + 2) % 3] = t1_opposite;


    adjacents[update_t2 * 3 + lupdate_op_t2] = t1;


    adjacents[update_t1 * 3 + lupdate_op_t1] = t2;

    adjacents[t1 * 3 + local_op_t1] = adjacents[t2 * 3 + (local_op_t2 + 1) % 3];
    adjacents[t2 * 3 + local_op_t2] = adjacents[t1 * 3 + (local_op_t1 + 1) % 3];

    adjacents[t1 * 3 + (local_op_t1 + 1) % 3] = t2;
    adjacents[t2 * 3 + (local_op_t2 + 1) % 3] = t1;

    vertexEnter[t1o] = t2;
    vertexEnter[t2o] = t1;


    // dont touche opposite + 2 on adjacents (see diagram)


}
Mesh::CirculatorFacesIterator& Mesh::CirculatorFacesIterator::operator++() {
    if(firstFace == -1) {
        firstFace = face;
    }

    int localVertexIndex = M->localIndex(vertex, face);
    if(localVertexIndex == -1) {
        std::cout << "CANT FIND NEXT" << std::endl;
        return * this;
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

    int localVertexIndex = M->localIndex(vertex, face);


    std::cout << "SOMMET = " << vertex << " F = " << face << " :: [" << M->triangles[face*3] << " " << M->triangles[face*3+1] << " " << M->triangles[face*3+2] << "] :: [" << M->adjacents[face*3] << " " << M->adjacents[face*3+1] << " " << M->adjacents[face*3+2] << "]" << std::endl;

    if(localVertexIndex == -1) {
        std::cout << "CANT FIND NEXT" << std::endl;
        exit(0);
    }

    face = M->adjacents[face * 3 + ((localVertexIndex + 2) % 3)];
    return *this;
}

int Mesh::CirculatorVertexIterator::operator*() const  {
    int localVertexIndex = M->localIndex(vertex, face);
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
float epsilon = 0.0001;
int Mesh::findTriangle(const vec3 & p) {

    if(p.x < aaBox[0].x || p.x > aaBox[1].x || p.y < aaBox[0].y || p.y > aaBox[1].y) {
        return -1;
    }


    for(auto it = beginFacesIterator(); it != endFacesIterator(); ++it) {
        int f = *it;
        vec3 p1 = vertices[triangles[f]];
        vec3 p2 = vertices[triangles[f+1]];
        vec3 p3 = vertices[triangles[f+2]];
        if(p.z <= p1.z - epsilon || p.z <= p2.z - epsilon || p.z <= p3.z - epsilon || p.z >= p1.z + epsilon || p.z >= p2.z + epsilon || p.z >= p3.z + epsilon) {
            //std::cout << "skip " << f/3 << " for " << p.z << " with " << p1.z << " " << p2.z << " " << p3.z << std::endl;
            continue;
        }

        if(MathHelper::isInTriangle(p, p1, p2, p3)) {
            //std::cout << " is in " << f/3 << " with " << " " << p1.z << " " << p2.z << " " << p3.z << std::endl;
            return f/3;
        }
    }
    return -1;
}

void Mesh::addPointToTriangle(const vec3 & p, int t0) {

    this->vertices.push_back(p);

    this->colors.push_back((float)rand() / (float)RAND_MAX);
    this->colors.push_back((float)rand() / (float)RAND_MAX);
    this->colors.push_back((float)rand() / (float)RAND_MAX);

    int v0 = triangles[t0 * 3];
    int v1 = triangles[t0 * 3 + 1];
    int v2 = triangles[t0 * 3 + 2];

    int vn = vertices.size() - 1;
    int t1 = triangles.size() / 3;
    int t2 = triangles.size() / 3 + 1;

    int a0 = adjacents[t0 * 3];
    int a1 = adjacents[t0 * 3 + 1];
    int a2 = adjacents[t0 * 3 + 2];

    int a1_op = findOppositePoint(a1, t0);
    int a2_op = findOppositePoint(a2, t0);

    adjacents[a1 * 3 + a1_op] = t2;
    adjacents[a2 * 3 + a2_op] = t1;

    triangles[t0 * 3] = vn;

    adjacents[t0 * 3 + 1] = t2;
    adjacents[t0 * 3 + 2] = t1;


    triangles.push_back(vn);
    triangles.push_back(v0);
    triangles.push_back(v1);

    adjacents.push_back(a2);
    adjacents.push_back(t0);
    adjacents.push_back(t2);

    triangles.push_back(vn);
    triangles.push_back(v2);
    triangles.push_back(v0);

    adjacents.push_back(a1);
    adjacents.push_back(t1);
    adjacents.push_back(t0);


    this->vertexEnter.push_back(t0);
    this->vertexEnter[v0] = t1;
    this->vertexEnter[v1] = t1;
    this->vertexEnter[v2] = t2;

    std::cout << "t0 = " << t0 << " t1 = " << t1 << " t2 = " << t2 << std::endl;
}

