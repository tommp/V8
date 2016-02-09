layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;

out vec2 frag_tex_coord;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    gl_Position = vec4(position, 1.0f);
    frag_tex_coord = tex_coord;
}