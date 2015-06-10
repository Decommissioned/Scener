#include "program_terrain.h"
#include "enum_attributelayout.h"

ProgramTerrain::ProgramTerrain(const string& folder)
        : Program(folder, "pterrain_v.shader", "pterrain_f.shader", {{ATTRIBUTE_POSITION, "a_position"}})
{}

ProgramTerrain::~ProgramTerrain()
{}
