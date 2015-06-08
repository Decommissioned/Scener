layout(std140)
uniform global
{
    mat4 g_view;
    mat4 g_projection;
    vec4 g_eye;
    vec4 g_ambient_light;
    vec4 g_attenuation;
    float g_render_distance;
};
