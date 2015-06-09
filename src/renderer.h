#ifndef RENDERER_HEADER
#define RENDERER_HEADER

#include "configuration.h"
#include "structure_scene.h"

#include <unordered_map>

#include "model_mesh.h"
#include "program_phong.h"
#include "program_uniformbuffer.h"

/*
*
* Directories containing the meshes, textures and shaders must be provided to the constructor, which will automatically discover the resources and load them
* Meshes must be stored in Wavefront's OBJ file format
* Textures must be stored in the PNG file format
* Shaders must be written in GLSL and comply to the version specified in the configuration file "configuration.h"
* 
* This class is not copyable or movable
* This class is not inheritable
* 
*/
class Renderer final
{
        struct Global // vec4 used instead of vec3 for padding, as required by (std140)
        {
                mat4 view;
                mat4 projection;
                vec4 eye;
                vec4 ambient_light;
                vec4 attenuation;
                float render_distance;
        };

        Global m_global;
        ProgramPhong m_program_phong;
        UniformBuffer m_ubo;
        std::unordered_map<string, ModelMesh> m_meshes;
        uint64_t ticks;

        void LoadResources(const string& meshes_dir, const string& textures_dir);

public:

        explicit Renderer(const string& meshes_dir, const string& textures_dir, const string& shaders_dir);
        Renderer() = delete;
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        const Renderer& operator=(const Renderer&) = delete;
        const Renderer& operator=(Renderer&&) = delete;

        ~Renderer();

        void Draw(const Scene& scene);

};

#endif // RENDERER_HEADER
