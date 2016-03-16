layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

out vec3 frag_normal;
out vec3 frag_position;
out vec2 frag_tex_coord;

uniform mat4 model;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    gl_Position = projection * view *  model * vec4(position, 1.0f);

    frag_position = vec3(model * vec4(position, 1.0f));

    //Optimize this later
    frag_normal = mat3(transpose(inverse(model))) * normal;  

    frag_tex_coord = vec2(tex_coord.x, tex_coord.y);
} 