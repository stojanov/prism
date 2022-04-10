#version 330

out vec4 FragColor;

in vec3 Color;
in vec2 UV;
in vec3 Normal;
in vec4 WorldPos;
in float MaxHeight;

uniform float height;

float inverseLerp(float a, float b, float v)
{
    return clamp((v - a) / (b - a), 0.f, 1.f);
}

void main()
{
    float h = inverseLerp(-MaxHeight, MaxHeight, WorldPos.y);

    //FragColor = vec4(Color.xyz, 1.f);
    FragColor = vec4(h, h, h, 1.f);
}