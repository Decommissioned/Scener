#ifndef STRUCTURE_TERRAIN_HEADER
#define STRUCTURE_TERRAIN_HEADER

#include "configuration.h"

struct Terrain
{
        int width;
        int height;
        float tileSize;
        float minimumHeight;
        float maximumHeight;
        float spacing;
        string seed;
        string texture;
};

#endif // STRUCTURE_TERRAIN_HEADER
