#version 330 core
in vec2 TexCoord;
in vec4 color;

out vec4 colorout;

uniform sampler2D texture;

void main()
{
    colorout = texture(texture, TexCoord);
}