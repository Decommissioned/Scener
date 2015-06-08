#include "renderer.h"
#include "loader.h"

#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#define ncase break; case

static const vec3 x = {1.0f, 0.0f, 0.0f};
static const vec3 y = {0.0f, 1.0f, 0.0f};
static const vec3 z = {0.0f, 0.0f, 1.0f};

static const float rc = 0.0174532925f;
static const mat4 identity = mat4(1.0f);
static uint64_t timer = 0;

void Renderer::LoadResources(const string& meshes_dir, const string& textures_dir)
{
        auto files = ListFiles(meshes_dir, ".obj");
        for (auto& file : files)
        {
                auto mesh = LoadWavefrontObject(file);
                m_meshes.emplace(std::make_pair(mesh.nameID, mesh));
        }
}

Renderer::Renderer(const string& meshes_dir, const string& textures_dir, const string& shaders_dir) : m_program_phong(shaders_dir)
{
        LoadResources(meshes_dir, textures_dir);

        m_ubo.AddPrograms({m_program_phong.ID()}, "global");

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
}

Renderer::~Renderer()
{}

void Renderer::Draw(const Scene& scene)
{
        Global uniforms;
        uniforms.eye = vec4(scene.camera.eye, 1.0f);
        uniforms.view = glm::lookAt(scene.camera.eye, scene.camera.lookAt, scene.camera.up);
        uniforms.render_distance = 1000.0f;
        uniforms.projection = glm::perspectiveFov(scene.window.fov, float(scene.window.width), float(scene.window.height), 0.025f, 1000.0f);
        uniforms.ambient_light = vec4(scene.ambientLight, 0.0f);
        uniforms.attenuation = vec4(scene.attenuationFactors, 0.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_program_phong.Bind();
        m_ubo.Update(uniforms);

        for (auto& obj : scene.objects)
        {
                auto& itr = m_meshes.find(obj.mesh);
                if (itr != m_meshes.end())
                {
                        vec3 offset = {0.0f, 0.0f, 0.0f};
                        switch (obj.driver)
                        {
                                ncase Driver::ROTATE: offset = obj.driverSpeed * float(timer);
                        }

                        mat4 rotation = glm::rotate(glm::rotate(glm::rotate(identity, (obj.rotate.x + offset.x) * rc, x), (obj.rotate.y + offset.y) * rc, y), (obj.rotate.z + offset.z) * rc, z);
                        m_program_phong.LoadModel(glm::translate(glm::scale(identity, obj.scale), obj.translate) * rotation);
                        m_program_phong.LoadNormal(rotation);

                        m_program_phong.LoadKa(obj.ka);
                        m_program_phong.LoadKd(obj.kd);
                        m_program_phong.LoadKs(obj.ks);

                        m_program_phong.LoadDirectionalLights(scene.directionalLights);
                        m_program_phong.LoadPointLights(scene.pointLights);

                        itr->second.Draw();
                }
        }

        timer++;
        assert(glGetError() == 0);
}

void Renderer::ViewMatrix(const vec3& eye, const vec3& look_at, const vec3& up)
{
        m_global.eye = vec4(eye, 1.0);
        m_global.view = glm::lookAt(eye, look_at, up);
}

void Renderer::PerspectiveMatrix(float aspect, float fovy, float far)
{
        m_global.render_distance = far;
        m_global.projection = glm::perspectiveFov(fovy, aspect, 1.0f, 0.025f, far);
}