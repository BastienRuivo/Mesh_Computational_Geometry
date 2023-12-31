#include "meshhelper.h"

void MeshHelper::saveToOffFile(const Mesh & mesh, const std::string & name) {
    // open or create a file to write into
    std::cout << "Saving mesh to file " << name << ".off" << std::endl;
    std::ofstream offFile(name.c_str() + std::string(".off"));
    if (!offFile.is_open()) {
        std::cout << "Can't create file" << std::endl;
        std::cerr << "Error: cannot open file " << name << std::endl;
        return;
    }

    std::cout << "Writing mesh to file " << name << std::endl;

    // write the header
    offFile << "OFF" << std::endl;
    offFile << mesh.vertices.size() << " " << mesh.triangles.size() << " 0" << std::endl;

    for (size_t i = 0; i < mesh.vertices.size(); i++)
    {
        offFile << mesh.vertices[i].x << " " << mesh.vertices[i].y << " " << mesh.vertices[i].z << std::endl;
    }

    for (size_t i = 0; i < mesh.triangles.size(); i+=3)
    {
        offFile << "3 " << mesh.triangles[i] << " " << mesh.triangles[i+1] << " " << mesh.triangles[i+2] << std::endl;
    }

    // close the file
    std::cout << "Mesh saved" << std::endl;
    offFile.close();
}

void MeshHelper::loadFromOff(Mesh & mesh, const std::string & path) {
    // open file to read from if it exists
    mesh.clear();

    std::cout << "Loading mesh from file " << path << std::endl;
    std::ifstream offFile(path.c_str());

    if (!offFile.is_open()) {
        std::cerr << "Error: cannot open file " << path << std::endl;
        return;
    }

    // read the header
    std::string header;
    offFile >> header;
    if (header != "OFF") {
        std::cerr << "Error: invalid OFF file format" << std::endl;
        return;
    }



    int nbVertices, nbTriangles, nbEdges;

    offFile >> nbVertices >> nbTriangles >> nbEdges;

    std::cout << "Header :: nbVertices " << nbVertices << " nbTriangles " << nbTriangles << " nbEdges " << nbEdges << std::endl;

    mesh.vertices.push_back(vec3(0.5, 0.5, -1));
    mesh.colors.push_back(0.75);
    mesh.colors.push_back(0.0);
    mesh.colors.push_back(0.75);

    for(int i = 0; i < nbVertices; i++) {
        double x, y, z;
        offFile >> x >> y >> z;
        mesh.vertices.push_back(vec3(x, y, z));
        mesh.colors.push_back(1.0);
        mesh.colors.push_back(1.0);
        mesh.colors.push_back(1.0);
        if(mesh.aaBox[0].x > x) mesh.aaBox[0].x = x;
        else if(mesh.aaBox[1].x < x) mesh.aaBox[1].x = x;
        if(mesh.aaBox[0].y > y) mesh.aaBox[0].y = y;
        else if(mesh.aaBox[1].y < y) mesh.aaBox[1].y = y;
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
                    mesh.triangles.push_back(value + 1);
                }
            }



            i++;
        }
        //std::cout << "SIZE :: " << triangles.size() << std::endl;
        //std::cout << triangles[triangles.size()-3] << " " << triangles[triangles.size()-2] << " " << triangles[triangles.size()-1] << std::endl;
    }

    mesh.paintAdjacents();
}

void MeshHelper::generatePyramide() {

    Mesh mesh;

    mesh.clear();

    mesh.vertices.push_back(vec3(0,0,0));
    mesh.vertices.push_back(vec3(1,0,0));
    mesh.vertices.push_back(vec3(0,0,1));
    mesh.vertices.push_back(vec3(1,0,1));

    //SOMMET
    mesh.vertices.push_back(vec3(0.5,1,0.5));

    //FACE

    //FACE 1
    mesh.triangles.push_back(0);
    mesh.triangles.push_back(1);
    mesh.triangles.push_back(2);

    mesh.colors.push_back(1.0);
    mesh.colors.push_back(0.0);
    mesh.colors.push_back(0.0);

    //FACE 2
    mesh.triangles.push_back(2);
    mesh.triangles.push_back(1);
    mesh.triangles.push_back(3);

    mesh.colors.push_back(1.0);
    mesh.colors.push_back(0.0);
    mesh.colors.push_back(0.0);

    //FACE 3
    mesh.triangles.push_back(0);
    mesh.triangles.push_back(4);
    mesh.triangles.push_back(1);

    mesh.colors.push_back(0.0);
    mesh.colors.push_back(1.0);
    mesh.colors.push_back(0.0);

    //FACE 4
    mesh.triangles.push_back(0);
    mesh.triangles.push_back(4);
    mesh.triangles.push_back(2);

    mesh.colors.push_back(0.0);
    mesh.colors.push_back(0.0);
    mesh.colors.push_back(1.0);

    //FACE 5
    mesh.triangles.push_back(2);
    mesh.triangles.push_back(4);
    mesh.triangles.push_back(3);

    mesh.colors.push_back(0.0);
    mesh.colors.push_back(1.0);
    mesh.colors.push_back(1.0);

    //FACE 6

    mesh.triangles.push_back(1);
    mesh.triangles.push_back(4);
    mesh.triangles.push_back(3);

    mesh.colors.push_back(1.0);
    mesh.colors.push_back(1.0);
    mesh.colors.push_back(0.0);

    saveToOffFile(mesh, "Pyramide");
}

void MeshHelper::generateTetrahedron() {
    Mesh mesh;
    mesh.clear();
    mesh.vertices.push_back(vec3(0,0,0));
    mesh.vertices.push_back(vec3(0,0,1));
    mesh.vertices.push_back(vec3(1,0,1));
    mesh.vertices.push_back(vec3(0.5,1,0.5));

    //Base
    mesh.triangles.push_back(0);
    mesh.triangles.push_back(2);
    mesh.triangles.push_back(1);

    mesh.triangles.push_back(1);
    mesh.triangles.push_back(2);
    mesh.triangles.push_back(3);

    mesh.triangles.push_back(3);
    mesh.triangles.push_back(0);
    mesh.triangles.push_back(2);

    mesh.triangles.push_back(0);
    mesh.triangles.push_back(3);
    mesh.triangles.push_back(1);

    saveToOffFile(mesh, "Tetrahedron");
}

bool MeshHelper::offExist(const std::string & name) {
    // check if name.off exist

    std::ifstream offFile(name.c_str() + std::string(".off"));
    return offFile.is_open();
}

void MeshHelper::generateCircle() {
    Mesh mesh;
    mesh.clear();

    int nbPoints = 60;

    float angle = 360.f / nbPoints;

    vec3 center(0,0,0);

    for(int i = 0; i < nbPoints; i++) {
        float rad = angle * i * M_PI / 180.f;
        mesh.vertices.push_back(vec3(cos(rad), 0, sin(rad)));
    }

    for(int i = 0; i < nbPoints - 2; i++) {
        mesh.triangles.push_back(0);
        mesh.triangles.push_back(i+1);
        mesh.triangles.push_back(i+2);
    }

    saveToOffFile(mesh, "Circle");
}

