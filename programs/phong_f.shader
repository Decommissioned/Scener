#pragma include("global.shader")

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;

out vec4 color;

uniform sampler2D sampler;
uniform bool u_enable_texture;

uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;

uniform int u_dl_count;
uniform vec3 u_dl_direction[MAX_LIGHTS];
uniform vec3 u_dl_color[MAX_LIGHTS];

uniform int u_pl_count;
uniform vec3 u_pl_position[MAX_LIGHTS];
uniform vec3 u_pl_color[MAX_LIGHTS];
uniform vec3 u_pl_attenuation[MAX_LIGHTS];

vec3 normal = normalize(v_normal);
vec3 to_eye = normalize(g_eye.xyz - v_position);

float attenuation(float dist, vec3 light_attenuation)
{
    return 1.0 / (light_attenuation.x + light_attenuation.y * dist + light_attenuation.z * light_attenuation.z * dist);
}

vec3 specular(vec3 light_dir)
{
    vec3 reflection = reflect(light_dir, normal);
    float factor = dot(reflection, to_eye);
    vec3 light = u_ks * pow(max(factor, 0.0), 2.0);

    return light;
}

vec3 directional(vec3 light_dir, vec3 light_color)
{
    float factor = dot(-light_dir, normal);
    vec3 diffuse = u_kd * max(factor, 0.0);

    return (diffuse + specular(light_dir)) * light_color;
}

vec3 point(vec3 light_pos, vec3 light_color, vec3 light_attenuation)
{
    vec3 from_light = v_position - light_pos;
    vec3 light_dir = normalize(from_light);
    float att = attenuation(length(from_light), light_attenuation);

    return directional(light_dir, light_color) * max(att, 0.0);
}

void main()
{
    vec3 total = vec3(0.0);
    for (int i = 0; i < min(MAX_LIGHTS, u_dl_count); i++) total += directional(u_dl_direction[i], u_dl_color[i]);
    for (int i = 0; i < min(MAX_LIGHTS, u_pl_count); i++) total += point(u_pl_position[i], u_pl_color[i], u_pl_attenuation[i]);

    vec3 result = (u_ka * g_ambient_light.xyz) + total;
    color = vec4(result, 1.0);
    if (u_enable_texture)
    {
        color *= texture(sampler, v_uv);
    }
}
