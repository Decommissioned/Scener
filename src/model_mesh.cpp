#include "model_mesh.h"
#include "loader_obj.h"
#include "enum_attributelayout.h"

#include <GL/glew.h>

void ModelMesh::bind(uint32_t vao)
{
        if (vao != m_bound_vao)
        {
                glBindVertexArray(vao);
                m_bound_vao = vao;
        }
}

ModelMesh::ModelMesh(const Mesh& data)
{
        m_vertices = data.indices.size();
        m_name = data.nameID;

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(m_attributes, m_vbo);

        bind(m_vao);

        if (data.indices.size() != 0)
        {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[0]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * data.indices.size(), &data.indices.front(), GL_STATIC_DRAW);
        }
        else HandleError("Mesh does not provide indices", ErrorCriticality::MEDIUM);

        if (data.positions.size() != 0)
        {
                glBindBuffer(GL_ARRAY_BUFFER, m_vbo[ATTRIBUTE_POSITION + 1]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.positions.size(), &data.positions.front(), GL_STATIC_DRAW);
                glVertexAttribPointer(ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(ATTRIBUTE_POSITION);
        }
        else HandleError("Mesh does not provide positions", ErrorCriticality::MEDIUM);

        if (data.uvs.size() != 0) // Positions and normals are required, texture coordinates are optional for now
        {
                glBindBuffer(GL_ARRAY_BUFFER, m_vbo[ATTRIBUTE_UV + 1]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.uvs.size(), &data.uvs.front(), GL_STATIC_DRAW);
                glVertexAttribPointer(ATTRIBUTE_UV, 2, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(ATTRIBUTE_UV);
        }
        else glDisableVertexAttribArray(ATTRIBUTE_UV);

        if (data.normals.size() != 0)
        {
                glBindBuffer(GL_ARRAY_BUFFER, m_vbo[ATTRIBUTE_NORMAL + 1]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.normals.size(), &data.normals.front(), GL_STATIC_DRAW);
                glVertexAttribPointer(ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(ATTRIBUTE_NORMAL);
        }
        else glDisableVertexAttribArray(ATTRIBUTE_NORMAL);
}

ModelMesh::~ModelMesh()
{
        glDeleteBuffers(m_attributes, m_vbo);
        glDeleteVertexArrays(1, &m_vao);
}

void ModelMesh::Bind() const
{
        bind(m_vao);
}

void ModelMesh::Draw() const
{
        bind(m_vao);
        glDrawElements(GL_TRIANGLES, m_vertices, GL_UNSIGNED_INT, 0);
}

uint32_t ModelMesh::ID() const
{
        return m_vao;
}

const string& ModelMesh::Name() const
{
        return m_name;
}

uint32_t ModelMesh::m_bound_vao = 0;