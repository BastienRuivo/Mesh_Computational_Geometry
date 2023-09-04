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
void GeometricWorld::draw() {
    glColor3d(1,0,0);
    glBegin(GL_TRIANGLES);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[1]);
    glPointDraw(_bBox[2]);
    glEnd();

    glColor3d(0,1,0);
    glBegin(GL_TRIANGLES);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[2]);
    glPointDraw(_bBox[3]);
    glEnd();

    glColor3d(0,0,1);
    glBegin(GL_TRIANGLES);
    glPointDraw(_bBox[0]);
    glPointDraw(_bBox[3]);
    glPointDraw(_bBox[1]);
    glEnd();

    _mesh.drawMesh();

    //glColor3d(1,1,0);
}

//Example with a wireframe bBox
void GeometricWorld::drawWireFrame() {
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

void Mesh::initTetrahedron() {

    std::cout<<"Make tetrahedron"<<std::endl;

    vertices.clear();
    triangles.clear();

    std::cout<<"Base..."<<std::endl;
    vertices.push_back(Point(0,0,0));
    vertices.push_back(Point(1,0,0));
    vertices.push_back(Point(0,0,1));

    //SOMMET
    std::cout<<"Sommet..."<<std::endl;
    vertices.push_back(Point(0,1,0));

    //FACE

    //FACE 1
    std::cout<<"Faces..."<<std::endl;
    triangles.push_back(0);
    triangles.push_back(1);
    triangles.push_back(2);

    //FACE 2
    triangles.push_back(2);
    triangles.push_back(1);
    triangles.push_back(3);

    //FACE 3
    triangles.push_back(0);
    triangles.push_back(2);
    triangles.push_back(3);

    //FACE 4
    triangles.push_back(0);
    triangles.push_back(3);
    triangles.push_back(1);

    _name = "Tetrahedron";

}


void Mesh::initPyramide() {
    vertices.push_back(Point(0,0,0));
    vertices.push_back(Point(1,0,0));
    vertices.push_back(Point(0,0,1));
    vertices.push_back(Point(1,0,1));
    
    //SOMMET
    vertices.push_back(Point(0,1,0));

    //FACE

    //FACE 1
    triangles.push_back(0);
    triangles.push_back(1);
    triangles.push_back(2);

    //FACE 2
    triangles.push_back(2);
    triangles.push_back(1);
    triangles.push_back(3);

    //FACE 3
    triangles.push_back(0);
    triangles.push_back(2);
    triangles.push_back(4);

    //FACE 4
    triangles.push_back(1);
    triangles.push_back(3);
    triangles.push_back(4);

    //FACE 5
    triangles.push_back(0);
    triangles.push_back(4);
    triangles.push_back(3);

    _name = "Pyramide";

}

void Mesh::initBBox() {

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

void Mesh::drawMesh() {
    for (size_t i = 0; i < triangles.size(); i++)
    {
        if(i % 3 == 0) {
            glBegin(GL_TRIANGLES);
        }
        glPointDraw(vertices[triangles[i]]);
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
        offFile << "3 " << triangles[i] << " " << triangles[i+1] << " " << triangles[i+2] << std::endl;
    }

    // close the file
    offFile.close();
}
