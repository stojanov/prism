#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCord;
layout(location = 2) in vec3 aNormal;

out vec2 TexCord;

uniform mat4 projectedview;
uniform mat4 transform;

void main()
{
    vec4 WorldPos = transform * vec4(aPos, 1.f);
    gl_Position = projectedview * WorldPos;
    TexCord = aTexCord;
}