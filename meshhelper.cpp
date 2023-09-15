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
        offFile << mesh.vertices[i]._x << " " << mesh.vertices[i]._y << " " << mesh.vertices[i]._z << std::endl;
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

    for(int i = 0; i < nbVertices; i++) {
        double x, y, z;
        offFile >> x >> y >> z;
        mesh.vertices.push_back(Point(x, y, z));
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
                    mesh.triangles.push_back(value);
                } else {
                    mesh.colors.push_back(value);
                }
            }



            i++;
        }
        //std::cout << "SIZE :: " << triangles.size() << std::endl;
        //std::cout << triangles[triangles.size()-3] << " " << triangles[triangles.size()-2] << " " << triangles[triangles.size()-1] << std::endl;
    }

    if(mesh.colors.size() != mesh.triangles.size()) {
        mesh.colors.resize(mesh.triangles.size());
        for(int i = 0; i < mesh.colors.size(); i+=3) {
            float c = (float)(i + 1) / (mesh.triangles.size());
            mesh.colors[i] = c;
            mesh.colors[i+1] = c;
            mesh.colors[i+2] = c;
        }
    }

    mesh.paintAdjacents();
    mesh.visitAll(0);
}

void MeshHelper::generatePyramide() {

    Mesh mesh;

    mesh.clear();

    mesh.vertices.push_back(Point(0,0,0));
    mesh.vertices.push_back(Point(1,0,0));
    mesh.vertices.push_back(Point(0,0,1));
    mesh.vertices.push_back(Point(1,0,1));

    //SOMMET
    mesh.vertices.push_back(Point(0.5,1,0.5));

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
    mesh.vertices.push_back(Point(0,0,0));
    mesh.vertices.push_back(Point(0,0,1));
    mesh.vertices.push_back(Point(1,0,1));
    mesh.vertices.push_back(Point(0.5,1,0.5));

    mesh.triangles.push_back(0);
    mesh.triangles.push_back(1);
    mesh.triangles.push_back(2);

    mesh.triangles.push_back(0);
    mesh.triangles.push_back(1);
    mesh.triangles.push_back(3);

    mesh.triangles.push_back(0);
    mesh.triangles.push_back(3);
    mesh.triangles.push_back(2);

    mesh.triangles.push_back(1);
    mesh.triangles.push_back(3);
    mesh.triangles.push_back(2);

    saveToOffFile(mesh, "Tetrahedron");
}

bool MeshHelper::offExist(const std::string & name) {
    // check if name.off exist

    std::ifstream offFile(name.c_str() + std::string(".off"));
    return offFile.is_open();
}
