layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 normal;

out vec4 vertex_color;
out vec2 tex_coord_frag;
uniform mat4 model;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    tex_coord_frag = vec2(tex_coord.x, 1.0 - tex_coord.y);
    vertex_color = color;
}