layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

out vec2 frag_tex_coord;
out vec3 frag_position;
out vec3 frag_normal;

uniform mat4 model;
uniform mat3 normal_model;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
    mat4 unrotated_view;
};

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);

    frag_tex_coord = tex_coord;

    frag_normal = normal_model * normal;

    frag_position = vec3(model * vec4(position, 1.0f));

}