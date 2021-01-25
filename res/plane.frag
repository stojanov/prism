#version 330 core
out vec4 OutColor;

in vec2 TexCord;
uniform sampler2D tex;

void main()
{
    OutColor = texture(tex, TexCord);
}