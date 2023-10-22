#version 400 core
out vec4 OutColor;

uniform float lightIntens;
uniform vec3 lightClr;
uniform sampler2D textureAtlas;

in vec3 ToLightVec;
in vec3 Normal;
in vec3 Color;
in vec2 TexCoord;

void main()
{
    vec3 uNormal = normalize(Normal);
    vec3 uToLight = normalize(ToLightVec);

    float diff = max(dot(uNormal, uToLight), 0.f) * lightIntens;

    //OutColor = vec4(texture(tex, TexCord).xyz * (lightClr * diff), 1.f);
    //OutColor = vec4(1.f, 1.f, 1.f, 1.f);


    OutColor = texture(textureAtlas, TexCoord);

    //OutColor = vec4(Color * (lightClr * diff), 1.f);
}