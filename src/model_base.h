#ifndef MODEL_BASE_HEADER
#define MODEL_BASE_HEADER

#include "configuration.h"

class ModelBase
{

        static uint32_t m_bound_vao;

protected:

        uint32_t m_vao;

        void bind() const;

};

#endif // MODEL_BASE_HEADER
