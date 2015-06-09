#ifndef PROGRAM_PHONG_HEADER
#define PROGRAM_PHONG_HEADER

#include "program.h"
#include "structure_directionallight.h"
#include "structure_pointlight.h"

#include <vector>

class ProgramPhong final : public Program
{
        static const size_t MAX_LIGHTS = 4;

        int32_t m_location_model;
        int32_t m_location_normal;

        int32_t m_location_ka;
        int32_t m_location_kd;
        int32_t m_location_ks;

        int32_t m_location_dl_count;
        int32_t m_location_dl_direction[MAX_LIGHTS];
        int32_t m_location_dl_color[MAX_LIGHTS];

        int32_t m_location_pl_count;
        int32_t m_location_pl_position[MAX_LIGHTS];
        int32_t m_location_pl_color[MAX_LIGHTS];
        int32_t m_location_pl_attenuation[MAX_LIGHTS];

public:

        ProgramPhong(const string& folder);
        ~ProgramPhong();

        void LoadModel(const mat4& model);
        void LoadNormal(const mat4& normal);

        void LoadKa(const vec3& ka);
        void LoadKd(const vec3& kd);
        void LoadKs(const vec3& ks);

        void LoadDirectionalLights(const std::vector<DirectionalLight> lights);
        void LoadPointLights(const std::vector<PointLight> lights);

};

#endif // PROGRAM_PHONG_HEADER
