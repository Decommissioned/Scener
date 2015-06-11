#include "enum_attributelayout.h"
#include "procedural_terrain.h"

#include <vector>

#include <gl/glew.h>

ProceduralTerrain::ProceduralTerrain(const string& seed)
{

        const float spacing = 3.0f;
        const size_t size = 10;

        float delta2 = float(size) * spacing / 2.0f;

        std::vector<float> grid;
        grid.reserve(size * size * 2);
        std::vector<uint32_t> indices;
        indices.reserve((size - 1) * (size - 1) * 6);

        float z = -delta2;
        for (size_t k = 0; k < size; k++, z+=spacing)
        {
                float x = -delta2;
                for (size_t i = 0; i < size; i++, x+=spacing)
                {
                        grid.emplace_back(x);
                        grid.emplace_back(z);
                }
        }

        for (size_t k = 0; k < (size - 1); k++)
        {
                for (size_t i = 0; i < (size - 1); i++)
                {
                        indices.emplace_back(size * k + i + size);
                        indices.emplace_back(size * k + i + 1);
                        indices.emplace_back(size * k + i);
                        indices.emplace_back(size * k + i + size);
                        indices.emplace_back(size * k + i + size + 1);
                        indices.emplace_back(size * k + i + 1);
                }
        }
        m_vertices = indices.size();

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