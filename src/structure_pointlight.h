#ifndef STRUCTURE_POINTLIGHT_HEADER
#define STRUCTURE_POINTLIGHT_HEADER

#include "configuration.h"

struct PointLight
{
        vec3 color;
        vec3 position;
        vec3 attenuation; // x - constant attenuation, y - linear attenuation, z - exponential attenuation
};

#endif // STRUCTURE_POINTLIGHT_HEADER
