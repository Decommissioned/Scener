#include "enum_attributelayout.h"
#include "model_mesh.h"
#include "loader_obj.h"

#include <GL/glew.h>

ModelMesh::ModelMesh(const Mesh& data)
{
        m_vertices = data.indices.size();

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(m_attributes, m_vbo);

        bind();

        if (data.indices.size() != 0)
        {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[0]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * data.indices.size(), &data.indices.front(), GL_STATIC_DRAW);
        }
        else HandleError("Mesh " + data.name + " does not provide indices", ErrorCriticality::MEDIUM);

        if (data.positions.size() != 0)
        {
                glBindBuffer(GL_ARRAY_BUFFER, m_vbo[ATTRIBUTE_POSITION + 1]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.positions.size(), &data.positions.front(), GL_STATIC_DRAW);
                glVertexAttribPointer(ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(ATTRIBUTE_POSITION);
        }
        else HandleError("Mesh " + data.name + " does not provide positions", ErrorCriticality::MEDIUM);

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

void ModelMesh::Draw() const
{
        bind();
        glDrawElements(GL_TRIANGLES, m_vertices, GL_UNSIGNED_INT, 0);
}