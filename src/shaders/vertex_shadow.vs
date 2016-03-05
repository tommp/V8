layout (location = 0) in vec3 position;

out vec2 frag_tex_coord;
out vec3 frag_position;
out vec3 frag_normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
}