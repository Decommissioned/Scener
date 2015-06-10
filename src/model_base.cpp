#include "model_base.h"

#include <gl/glew.h>

uint32_t ModelBase::m_bound_vao = 0;

void ModelBase::bind() const
{
        if (m_vao != m_bound_vao)
        {
                glBindVertexArray(m_vao);
                m_bound_vao = m_vao;
        }
}
