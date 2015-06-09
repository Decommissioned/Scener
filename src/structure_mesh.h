#ifndef STRUCTURE_MESH_HEADER
#define STRUCTURE_MESH_HEADER

#include "configuration.h"
#include <vector>

struct Mesh
{
        std::vector<size_t> indices;
        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<float> uvs;
        string name;
};

#endif // STRUCTURE_MESH_HEADER
