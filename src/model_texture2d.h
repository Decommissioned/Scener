#ifndef MODEL_TEXTURE2D_HEADER
#define MODEL_TEXTURE2D_HEADER

#include "configuration.h"
#include "structure_image.h"

class Texture2D
{

        static uint32_t m_active_texture;
        static uint32_t m_bound_texture;

        uint32_t m_texture;

public:

        Texture2D(const Image& image);
        ~Texture2D();

        static void Unbind();

        void Bind() const;
        uint32_t ID() const;

};

#endif // MODEL_TEXTURE2D_HEADER
