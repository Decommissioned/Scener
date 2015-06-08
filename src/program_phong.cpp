#include "program_phong.h"
#include "enum_attributelayout.h"

#include <algorithm>

ProgramPhong::ProgramPhong(const string& folder)
        : Program(folder, "phong_v.shader", "phong_f.shader", {{ATTRIBUTE_POSITION, "a_position"}, {ATTRIBUTE_NORMAL, "a_normal"}})
{

        m_location_model = uniformLocation("u_model");
        m_location_normal = uniformLocation("u_normal");

        m_location_ka = uniformLocation("u_ka");
        m_location_kd = uniformLocation("u_kd");
        m_location_ks = uniformLocation("u_ks");

        m_location_dl_count = uniformLocation("u_dl_count");
        m_location_pl_count = uniformLocation("u_pl_count");

        for (size_t i = 0; i < MAX_LIGHTS; i++)
        {
                auto istr = std::to_string(i);
                m_location_dl_direction[i] = uniformLocation("u_dl_direction[" + istr + ']');
                m_location_dl_color[i] = uniformLocation("u_dl_color[" + istr + ']');
                m_location_pl_position[i] = uniformLocation("u_pl_position[" + istr + ']');
                m_location_pl_color[i] = uniformLocation("u_pl_color[" + istr + ']');
        }
}

ProgramPhong::~ProgramPhong()
{}

void ProgramPhong::LoadModel(const mat4& model)
{
        loadMat4(m_location_model, model);
}

void ProgramPhong::LoadNormal(const mat4& normal)
{
        loadMat4(m_location_normal, normal);
}

void ProgramPhong::LoadKa(const vec3& ka)
{
        loadVec3(m_location_ka, ka);
}

void ProgramPhong::LoadKd(const vec3& kd)
{
        loadVec3(m_location_kd, kd);
}

void ProgramPhong::LoadKs(const vec3& ks)
{
        loadVec3(m_location_ks, ks);
}

void ProgramPhong::LoadDirectionalLights(const std::vector<DirectionalLight> lights)
{
        size_t count = lights.size();
        loadInteger(m_location_dl_count, count);
        for (size_t i = 0; i < std::min(count, MAX_LIGHTS); i++)
        {
                loadVec3(m_location_dl_direction[i], lights[i].direction);
                loadVec3(m_location_dl_color[i], lights[i].color);
        }
}

void ProgramPhong::LoadPointLights(const std::vector<PointLight> lights)
{
        size_t count = lights.size();
        loadInteger(m_location_pl_count, count);
        for (size_t i = 0; i < std::min(count, MAX_LIGHTS); i++)
        {
                loadVec3(m_location_pl_position[i], lights[i].position);
                loadVec3(m_location_pl_color[i], lights[i].color);
        }
}
