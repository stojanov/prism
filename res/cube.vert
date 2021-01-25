#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCord;
layout(location = 2) in vec3 aNormal;

out vec2 TexCord;
out vec3 Normal;
out vec3 ToLightVec;

uniform mat4 projectedview;
uniform mat4 transform;
uniform vec3 lightPos;

void main()
{
    vec4 WorldPos = transform * vec4(aPos, 1.f);
    gl_Position = projectedview * WorldPos;
    TexCord = aTexCord;
    Normal = mat3(transpose(inverse(transform))) * aNormal;
    ToLightVec = lightPos - WorldPos.xyz;
}