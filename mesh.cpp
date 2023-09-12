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
    initTetrahedron();
}

void Mesh::clear() {
    vertices.clear();
    vertexEnter.clear();
    triangles.clear();
    colors.clear();
    adjacents.clear();
    visited.clear();
}

void Mesh::initTetrahedron() {

    std::cout<<"Make tetrahedron"<<std::endl;

    clear();

    std::cout<<"Base..."<<std::endl;
    vertices.push_back(Point(0,0,0));
    vertices.push_back(Point(1,0,0));
    vertices.push_back(Point(0,0,1));
    vertices.push_back(Point(0,1,0));

    //FACE

    //FACE 0
    std::cout<<"Faces..."<<std::endl;
    triangles.push_back(0);
    triangles.push_back(1);
    triangles.push_back(2);

    colors.push_back(1.0);
    colors.push_back(0.0);
    colors.push_back(0.0);

    //FACE 1
    triangles.push_back(2);
    triangles.push_back(1);
    triangles.push_back(3);

    colors.push_back(0.0);
    colors.push_back(1.0);
    colors.push_back(0.0);

    //FACE 2
    triangles.push_back(0);
    triangles.push_back(2);
    triangles.push_back(3);

    colors.push_back(0.0);
    colors.push_back(0.0);
    colors.push_back(1.0);

    //FACE 3
    triangles.push_back(0);
    triangles.push_back(3);
    triangles.push_back(1);

    colors.push_back(1.0);
    colors.push_back(1.0);
    colors.push_back(0.0);

    _name = "Tetrahedron";

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

    std::cout << "Number of edges : " << map.size() << std::endl;

    visited.resize(triangles.size()/3, false);

//    for (int t = 0; t < triangles.size(); t+=3) {
//        std::cout << "triangle["<< t/3 << "] = [" << adjacents[t] << ", " << adjacents[t+1] << ", " << adjacents[t+2] << "] "<< std::endl;
//    }

}


void Mesh::initPyramide() {

    clear();

    vertices.push_back(Point(0,0,0));
    vertices.push_back(Point(1,0,0));
    vertices.push_back(Point(0,0,1));
    vertices.push_back(Point(1,0,1));
    
    //SOMMET
    vertices.push_back(Point(0.5,1,0.5));

    //FACE

    //FACE 1
    triangles.push_back(0);
    triangles.push_back(1);
    triangles.push_back(2);

    colors.push_back(1.0);
    colors.push_back(0.0);
    colors.push_back(0.0);

    //FACE 2
    triangles.push_back(2);
    triangles.push_back(1);
    triangles.push_back(3);

    colors.push_back(1.0);
    colors.push_back(0.0);
    colors.push_back(0.0);

    //FACE 3
    triangles.push_back(0);
    triangles.push_back(4);
    triangles.push_back(1);

    colors.push_back(0.0);
    colors.push_back(1.0);
    colors.push_back(0.0);

    //FACE 4
    triangles.push_back(0);
    triangles.push_back(4);
    triangles.push_back(2);

    colors.push_back(0.0);
    colors.push_back(0.0);
    colors.push_back(1.0);

    //FACE 5
    triangles.push_back(2);
    triangles.push_back(4);
    triangles.push_back(3);

    colors.push_back(0.0);
    colors.push_back(1.0);
    colors.push_back(1.0);

    //FACE 6

    triangles.push_back(1);
    triangles.push_back(4);
    triangles.push_back(3);

    colors.push_back(1.0);
    colors.push_back(1.0);
    colors.push_back(0.0);

    _name = "Pyramide";

}

void Mesh::initBBox() {
    clear();
}

Mesh::~Mesh() {
    // Destructor automatically called before a Mesh is destroyed (default strategy)
}

std::string Mesh::getName() {
    return this->_name;
}

void Mesh::saveToOffFile() {
    this->saveToOffFile(this->_name);
}

void Mesh::drawMesh(bool useVisited) {

    for (size_t i = 0; i < triangles.size(); i++)
    {
        if(!useVisited || visited[i/3]) {
            if(i % 3 == 0) {
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
                glBegin(GL_LINE_STRIP);
                glColor3f(colors[i], colors[i+1], colors[i+2]);
            }
            glPointDraw(vertices[triangles[i]]);
        }
    }
    glEnd();
}

void Mesh::saveToOffFile(const std::string & name) {
    // open or create a file to write into
    std::cout << "Saving mesh to file " << name << std::endl;
    std::ofstream offFile(name.c_str() + std::string(".off"));
    if (!offFile.is_open()) {
        std::cout << "Can't create file" << std::endl;
        std::cerr << "Error: cannot open file " << name << std::endl;
        return;
    }

    std::cout << "Writing mesh to file " << name << std::endl;

    // write the header
    offFile << "OFF" << std::endl;
    offFile << vertices.size() << " " << triangles.size() << " 0" << std::endl;

    for (size_t i = 0; i < vertices.size(); i++)
    {
        offFile << vertices[i]._x << " " << vertices[i]._y << " " << vertices[i]._z << std::endl;
    }

    for (size_t i = 0; i < triangles.size(); i+=3)
    {
        offFile << "3 " << triangles[i] << " " << triangles[i+1] << " " << triangles[i+2] << " " << colors[i] << " " << colors[i+1] << " " << colors[i+2] << std::endl;
    }

    // close the file
    offFile.close();
}

void Mesh::loadFromOff(const std::string & path) {
    // open file to read from if it exists
    clear();

    std::cout << "Loading mesh from file " << path << std::endl;
    std::ifstream offFile(path.c_str());

    if (!offFile.is_open()) {
        std::cerr << "Error: cannot open file " << path << std::endl;
        return;
    }

    std::cout << "Reading mesh from file " << path << std::endl;

    // read the header
    std::string header;
    offFile >> header;
    if (header != "OFF") {
        std::cerr << "Error: invalid OFF file format" << std::endl;
        return;
    }

    int nbVertices, nbTriangles, nbEdges;

    offFile >> nbVertices >> nbTriangles >> nbEdges;

    for(int i = 0; i < nbVertices; i++) {
        double x, y, z;
        offFile >> x >> y >> z;
        vertices.push_back(Point(x, y, z));
    }

    std::string line;
    while(std::getline(offFile, line)) {
        int i = 0;
        int nbTri = -1;
        int value;

        std::istringstream iss(line);

        while(iss >> value) {
            if(i == 0) {
                if(value != 3) {
                    std::cout << "Error: invalid OFF file format :: " << value << std::endl;
                    std::cout << "Complete Line :: "<< std::endl;
                    std::cout << line << std::endl;
                    return;
                } else {
                    nbTri = value;
                }
            } else {
                if(nbTri == 3) {
                    triangles.push_back(value);
                } else {
                    colors.push_back(value);
                }
            }



            i++;
        }
        //std::cout << "SIZE :: " << triangles.size() << std::endl;
        //std::cout << triangles[triangles.size()-3] << " " << triangles[triangles.size()-2] << " " << triangles[triangles.size()-1] << std::endl;
    }
//    float t = 0.0;
//    int i = 0;
    while(colors.size() < triangles.size()) {
        colors.push_back(rand()%255 / 255.f);
//        if(i == 2) {
//            i = 0;
//            t += 0.0001;
//        } else {
//            i++;
//        }
    }

    paintAdjacents();
    visitAll(0);
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
