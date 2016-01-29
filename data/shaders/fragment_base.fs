#version 330 core
in vec2 tex_coord;
in vec4 vertex_color;

out vec4 colorout;

uniform sampler2D texture;

void main()
{
    colorout = texture(texture, tex_coord) * vertex_color;
}