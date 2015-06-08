#ifndef STRUCTURE_OBJECT_HEADER
#define STRUCTURE_OBJECT_HEADER

#include "configuration.h"
#include "enum_drivers.h"

struct Object
{
        string mesh;
        string texture;
        
        vec3 translate;
        vec3 rotate;
        vec3 scale;
        
        vec3 ka;
        vec3 kd;
        vec3 ks;

        Driver driver;
        vec3 driverSpeed;
};

#endif // STRUCTURE_OBJECT_HEADER
