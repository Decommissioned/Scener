#ifndef STRUCTURE_SCENE_HEADER
#define STRUCTURE_SCENE_HEADER

#include "structure_skybox.h"
#include "structure_window.h"
#include "structure_camera.h"
#include "structure_terrain.h"
#include "structure_directionallight.h"
#include "structure_pointlight.h"
#include "structure_object.h"

#include <vector>

struct Scene
{
        bool perspective;
        vec3 ambientLight;
        vec3 clearColor;

        Skybox skybox;
        Window window;
        Camera camera;
        Terrain terrain;

        std::vector<DirectionalLight> directionalLights;
        std::vector<PointLight> pointLights;
        std::vector<Object> objects;
};

#endif // STRUCTURE_SCENE_HEADER
