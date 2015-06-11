#pragma include("global.shader")

in vec2 a_position;

out float h;

float random(vec2 coordinate)
{
    const vec2 seed = vec2(12.9898, 78.233);
    return fract(sin(dot(coordinate, seed)) * 43758.5453);
}

float height()
{
    const vec2 right = vec2(1.0, 0.0);
    const vec2 below = vec2(0.0, 1.0);

    vec2 discrete = floor(a_position);
    vec2 fraction = fract(a_position);

    float h0 = random(discrete);
    float h1 = random(discrete + right);
    float h2 = random(discrete + below);
    float h3 = random(discrete + right + below);

    float t0 = mix(h0, h1, fraction.x);
    float t1 = mix(h2, h3, fraction.x);
    return mix(t0, t1, fraction.y);
}

void main()
{
    float y = height();
    vec3 vertex = vec3(a_position.x, y * 2.0 - 2.0, a_position.y) * 2.5;
    h = y;
    gl_Position = g_projection * g_view * vec4(vertex, 1.0);
}
