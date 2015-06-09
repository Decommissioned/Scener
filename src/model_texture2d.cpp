#include "model_texture2d.h"

#include <GL/glew.h>

Texture2D::Texture2D(const Image& image)
{

        glGenTextures(1, &m_texture);

        Bind();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
                     image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     &image.data.front());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture2D::~Texture2D()
{
        glDeleteTextures(1, &m_texture);
}

void Texture2D::Unbind()
{
        if (m_bound_texture != 0)
        {
                glBindTexture(GL_TEXTURE_2D, 0);
                m_bound_texture = 0;
        }
}

void Texture2D::Bind() const
{
        if (m_bound_texture != m_texture)
        {
                glBindTexture(GL_TEXTURE_2D, m_texture);
                m_bound_texture = m_texture;
        }
}

uint32_t Texture2D::ID() const
{
        return m_texture;
}

uint32_t Texture2D::m_bound_texture = 0;
uint32_t Texture2D::m_active_texture = 0;
