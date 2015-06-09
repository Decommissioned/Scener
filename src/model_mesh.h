#ifndef MODEL_MESH_HEADER
#define MODEL_MESH_HEADER

#include "configuration.h"
#include "structure_mesh.h"

class ModelMesh final
{

        const static uint32_t m_attributes = 4;

        static uint32_t m_bound_vao;
        static void bind(uint32_t vao);

        uint32_t m_vao;
        uint32_t m_vbo[m_attributes];
        size_t m_vertices;

public:

        ModelMesh() = delete;
        ModelMesh(const Mesh& data);
        ModelMesh(const ModelMesh&) = delete;
        const ModelMesh& operator=(const ModelMesh&) = delete;

        ~ModelMesh();

        void Bind() const;
        void Draw() const;

        uint32_t ID() const;

};

#endif // MODEL_MESH_HEADER
