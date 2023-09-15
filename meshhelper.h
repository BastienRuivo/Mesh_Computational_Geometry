#ifndef MESHHELPER_H
#define MESHHELPER_H

#include <string>
#include "mesh.h"

class MeshHelper
{
public:
    MeshHelper() {}

    static void saveToOffFile(const Mesh & mesh, const std::string & name);
    static void loadFromOff(Mesh & mesh, const std::string & path);

    static bool offExist(const std::string & path);

    static void generatePyramide();
    static void generateTetrahedron();
};

#endif // MESHHELPER_H
