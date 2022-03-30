#version 330

out vec4 FragColor;

in vec3 Color;
in vec2 UV;
in vec3 Normal;

void main()
{
    FragColor = vec4(UV.xy, 0.f, 1.f);
}