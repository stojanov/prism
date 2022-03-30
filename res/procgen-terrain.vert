#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aClr;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aUV;

uniform mat4 projectedView;
uniform mat4 transform;

out vec3 Color;
out vec2 UV;
out vec3 Normal;

void main()
{
    vec4 worldPos = transform * vec4(aPos, 1.f);
    gl_Position = projectedView * worldPos;
    Color = aClr;
    UV = aUV;
    Normal = aNormal;
}