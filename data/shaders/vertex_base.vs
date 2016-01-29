#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 tex_coord;
layout (location = 2) in vec3 normal;

out vec4 vertex_color;
out vec2 tex_coord_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    tex_coord_out = vec3(tex_coord.x, 1.0 - tex_coord.y);
    vertex_color = color;
}