#ifndef MODEL_MESH_HEADER
#define MODEL_MESH_HEADER

#include "model_base.h"
#include "structure_mesh.h"

class ModelMesh final : private ModelBase
{

        const static uint32_t m_attributes = 4;

        uint32_t m_vbo[m_attributes];
        size_t m_vertices;

public:

        ModelMesh() = delete;
        ModelMesh(const Mesh& data);
        ModelMesh(const ModelMesh&) = delete;
        const ModelMesh& operator=(const ModelMesh&) = delete;

        ~ModelMesh();

        void Draw() const;

};

#endif // MODEL_MESH_HEADER
