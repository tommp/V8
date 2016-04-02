layout (location = 0) in vec3 position;

out vec3 color;
out vec3 frag_normal;
out vec3 frag_position; 

uniform mat4 models[100];
uniform vec3 colors[100];

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
    mat4 unrotated_view;
};

void main()
{
    gl_Position = projection * view * models[gl_InstanceID] * vec4(position, 1.0f);

    frag_normal = vec3(0.0, 1.0, 0.0);

    frag_position = vec3(models[gl_InstanceID] * vec4(position, 1.0f));

    color = colors[gl_InstanceID];
}