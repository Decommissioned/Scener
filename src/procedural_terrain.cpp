#include "enum_attributelayout.h"
#include "procedural_terrain.h"

#include <vector>

#include <gl/glew.h>

ProceduralTerrain::ProceduralTerrain(const string& seed)
{

        const float spacing = 1.0f;
        const size_t size = 100;
        const size_t sizei = size - 1;

        m_vertices = sizei * sizei * 6;

        float delta2 = float(size) * spacing / 2.0f;
        float step = spacing / float(size);

        std::vector<float> grid;
        grid.reserve(size * size * 2);
        std::vector<uint32_t> indices;
        indices.reserve(m_vertices);

        float z = -delta2;
        for (size_t k = 0; k < size; k++, z+=step)
        {
                float x = -delta2;
                for (size_t i = 0; i < size; i++, x+=step)
                {
                        grid.emplace_back(x);
                        grid.emplace_back(z);
                }
        }

        for (size_t k = 0; k < sizei; k++)
        {
                for (size_t i = 0; i < sizei; i++)
                {
                        indices.emplace_back(sizei * k + i);
                        indices.emplace_back(sizei * k + i + 1);
                        indices.emplace_back(sizei * k + i + sizei);
                        indices.emplace_back(sizei * k + i + 1);
                        indices.emplace_back(sizei * k + i + sizei + 1);
                        indices.emplace_back(sizei * k + i + sizei);
                }
        }

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(m_attributes, m_vbo);

        bind();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), &indices.front(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo[ATTRIBUTE_POSITION + 1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * grid.size(), &grid.front(), GL_STATIC_DRAW);
        glVertexAttribPointer(ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(ATTRIBUTE_POSITION);
}

ProceduralTerrain::~ProceduralTerrain()
{
        glDeleteBuffers(m_attributes, m_vbo);
        glDeleteVertexArrays(1, &m_vao);
}

void ProceduralTerrain::Draw() const
{
        bind();
        glDrawElements(GL_TRIANGLES, m_vertices, GL_UNSIGNED_INT, 0);
}