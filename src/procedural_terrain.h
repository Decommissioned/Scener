#ifndef PROCEDURAL_TERRAIN_HEADER
#define PROCEDURAL_TERRAIN_HEADER

#include "model_base.h"

class ProceduralTerrain : private ModelBase
{

        const static uint32_t m_attributes = 2;

        uint32_t m_vbo[m_attributes];
        size_t m_vertices;

public:

        ProceduralTerrain(const string& seed);
        ~ProceduralTerrain();

        void Draw() const;

};

#endif // PROCEDURAL_TERRAIN_HEADER
