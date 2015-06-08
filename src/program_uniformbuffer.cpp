#include "program_uniformbuffer.h"

#include <GL/glew.h>
#include <exception>

void UniformBuffer::update(const void* data, uint32_t length)
{
        glBindBuffer(GL_UNIFORM_BUFFER, m_buffer);
        glBufferData(GL_UNIFORM_BUFFER, length, data, GL_STREAM_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, m_binding_point, m_buffer);
}

UniformBuffer::UniformBuffer()
{
        if (m_binding_points == 0)
        {
                int32_t max;
                glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &max);
                if (max <= 0)
                {
                        throw std::runtime_error("Error querying maximum number of uniform buffer bindings");
                }
                m_binding_points = max;
        }
        if (m_next_binding_point >= m_binding_points)
        {
                throw std::runtime_error("No uniform buffer available");
        }
        m_binding_point = m_next_binding_point;
        m_next_binding_point++;
        glGenBuffers(1, &m_buffer);
}

UniformBuffer::~UniformBuffer()
{
        glDeleteBuffers(1, &m_buffer);
}

void UniformBuffer::AddPrograms(std::initializer_list<uint32_t> programs, const string& name)
{
        for (auto program : programs)
        {
                uint32_t block = glGetUniformBlockIndex(program, name.c_str());
                glUniformBlockBinding(program, block, m_binding_point);
        }
}

uint32_t UniformBuffer::m_binding_points = 0;
uint32_t UniformBuffer::m_next_binding_point = 0;