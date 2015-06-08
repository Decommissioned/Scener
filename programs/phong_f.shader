#pragma include("global.shader")

in vec3 v_position;
in vec3 v_normal;

out vec4 color;

uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;

uniform int u_dl_count;
uniform vec3 u_dl_direction[4];
uniform vec3 u_dl_color[4];

uniform int u_pl_count;
uniform vec3 u_pl_position[4];
uniform vec3 u_pl_color[4];

vec3 normal = normalize(v_normal);
vec3 to_eye = normalize(g_eye.xyz - v_position);

vec3 dl(vec3 direction, vec3 color)
{
    vec3 diffuse = u_kd * max( dot(direction, normal), 0.0 );

    vec3 reflection = reflect(-direction, normal);
    vec3 specular = u_ks * max( dot(reflection, to_eye), 0.0 );

    return (diffuse + specular) * color;
}

vec3 pl(vec3 position, vec3 color)
{
    vec3 to_light = position - v_position;
    vec3 direction = normalize(to_light);

    float dist = length(to_light);
    float attenuation = g_attenuation.x * g_attenuation.x * dist + g_attenuation.y * dist + g_attenuation.z;
    vec3 diffuse = u_kd * max( dot(direction, normal), 0.0 ) / attenuation;

    vec3 reflection = reflect(-direction, normal);
    vec3 specular = u_ks * max( dot(reflection, to_eye), 0.0 );

    return (diffuse + specular) * color;
}

void main()
{

    vec3 total = vec3(0.0);
    for (int i = 0; i < min(4, u_dl_count); i++)
        total += dl(u_dl_direction[i], u_dl_color[i]);

    for (int i = 0; i < min(4, u_pl_count); i++)
        total += pl(u_pl_position[i], u_pl_color[i]);

    vec3 result = u_ka * g_ambient_light.xyz + total;
    color = vec4(result, 1.0);
}
