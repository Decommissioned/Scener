layout(std140)
uniform global
{
    mat4 g_view;
    mat4 g_projection;
    vec3 g_eye;
    vec3 g_ambient_light;
    vec3 g_attenuation;
    float g_render_distance;
};
