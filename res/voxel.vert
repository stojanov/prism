#version 400 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

uniform mat4 transform;
uniform mat4 projectedview;
uniform vec3 lightPos;
uniform mat4 worldoffset;

out vec3 Normal;
out vec3 ToLightVec;
out vec3 Color;

void main()
{
    //vec4 WorldPos = worldoffset * transform * vec4(aPos, 1.f);
    vec4 WorldPos = transform * vec4(aPos, 1.f);
    gl_Position = projectedview * WorldPos;
    Normal = mat3(transpose(inverse(transform))) * aNormal;//aNormal;
    Color = aColor;
    ToLightVec = lightPos - WorldPos.xyz;
}