#pragma include("global.shader")

in float h;

out vec4 color;

void main()
{
    color = vec4(h, h, h, 1.0);
}
