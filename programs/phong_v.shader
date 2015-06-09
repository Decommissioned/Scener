#pragma include("global.shader")

in vec3 a_position;
in vec3 a_normal;
in vec2 a_uv;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_uv;

uniform mat4 u_model;
uniform mat4 u_normal;

void main()
{
    vec4 world = u_model * vec4(a_position, 1.0);
    vec4 clip = g_projection * g_view * world;
    vec4 normal = u_normal * vec4(a_normal, 0.0);

    gl_Position = clip;
    v_position = world.xyz;
    v_normal = normal.xyz;
    v_uv = a_uv;
}
