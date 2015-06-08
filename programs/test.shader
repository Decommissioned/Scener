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

struct DirectionalLight
{
        vec3 color;
        vec3 direction;
};

struct PointLight
{
        vec3 color;
        vec3 position;
};


in vec3 v_position;
in vec3 v_normal;

out vec4 color;

uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;

uniform int u_directional_light_count;
uniform int u_point_light_count;

uniform DirectionalLight u_directional_lights[4];
uniform PointLight u_point_lights[4];

vec3 dl (DirectionaLight light)
{

    vec3 direction = light.direction;
    
    vec3 diffuse = u_kd * max( dot(direction, v_normal), 0.0 );

    vec3 reflection = reflect(-direction, v_normal);
    vec3 to_eye = normalize(g_eye - v_position);
    vec3 specular = u_ks * max( dot(reflection, to_eye), 0.0 );

    return (diffuse + specular);

}

void main()
{
    vec3 result = u_ka * g_ambient_light + dl(u_directional_light[0]);
    color = vec4(result, 1.0);
}
