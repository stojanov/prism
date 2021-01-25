#version 330 core
out vec4 OutColor;

in vec2 TexCord;
in vec3 Normal;
in vec3 LightPos;
in vec3 ToLightVec;

uniform sampler2D tex;
uniform vec3 lightPos;
uniform float lightIntens;
uniform vec3 lightClr;

void main()
{
    vec3 uNormal = normalize(Normal);
    vec3 uToLight = normalize(ToLightVec);

    float diff = max(dot(uNormal, uToLight), 0.f) * lightIntens;

    OutColor = vec4(texture(tex, TexCord).xyz * (lightClr * diff), 1.f);
}