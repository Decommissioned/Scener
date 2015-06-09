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

void Renderer::LoadResources(const string& meshes_dir, const string& textures_dir)
{
        auto mesh_files = ListFiles(meshes_dir, ".obj");
        for (auto& file : mesh_files)
        {
                auto mesh = LoadWavefrontObject(file);
                m_meshes.emplace(std::make_pair(mesh.name, mesh));
        }
        auto texture_files = ListFiles(textures_dir, ".png");
        for (auto& file : texture_files)
        {
                auto texture = LoadPNG(file);
                m_textures.emplace(std::make_pair(texture.name, texture));
        }
}

Renderer::Renderer(const string& meshes_dir, const string& textures_dir, const string& shaders_dir) : m_program_phong(shaders_dir)
{
        LoadResources(meshes_dir, textures_dir);

        m_ubo.AddPrograms({m_program_phong.ID()}, "global");

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        ticks = 0;
}

Renderer::~Renderer()
{}

void Renderer::Draw(const Scene& scene)
{
        Global uniforms;
        uniforms.eye = vec4(scene.camera.eye, 1.0f);
        uniforms.view = glm::lookAt(scene.camera.eye, scene.camera.lookAt, scene.camera.up);
        uniforms.render_distance = 1000.0f;

        float aspect = float(scene.window.width) / float(scene.window.height);
        if (scene.perspective)
        {
                uniforms.projection = glm::perspectiveFov(scene.window.fov, aspect, 1.0f, 0.025f, uniforms.render_distance);
        }
        else
        {
                uniforms.projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, 0.025f, uniforms.render_distance);
        }
        uniforms.ambient_light = vec4(scene.ambientLight, 0.0f);

        glClearColor(scene.clearColor.x, scene.clearColor.y, scene.clearColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_program_phong.Bind();
        m_ubo.Update(uniforms);

        for (auto& item : scene.objects)
        {
                auto& mesh = m_meshes.find(item.mesh);
                if (mesh != m_meshes.end())
                {
                        vec3 offset = {0.0f, 0.0f, 0.0f};
                        switch (item.driver)
                        {
                                ncase Driver::ROTATE: offset = item.driverSpeed * float(ticks);
                        }

                        mat4 rotation = glm::rotate(glm::rotate(glm::rotate(identity, (item.rotate.x + offset.x) * rc, x), (item.rotate.y + offset.y) * rc, y), (item.rotate.z + offset.z) * rc, z);
                        m_program_phong.LoadModel(glm::scale(glm::translate(identity, item.translate), item.scale) * rotation);
                        m_program_phong.LoadNormal(rotation);

                        m_program_phong.LoadKa(item.ka);
                        m_program_phong.LoadKd(item.kd);
                        m_program_phong.LoadKs(item.ks);

                        m_program_phong.LoadDirectionalLights(scene.directionalLights);
                        m_program_phong.LoadPointLights(scene.pointLights);

                        auto& texture = m_textures.find(item.texture);
                        bool enable_texture = texture != m_textures.end();
                        if (enable_texture)
                        {
                                texture->second.Bind();
                        }
                        m_program_phong.EnableTexturing(enable_texture);

                        mesh->second.Draw();
                }
        }

        ticks++;
        assert(glGetError() == 0);
}