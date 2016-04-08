layout (location = 0) in vec3 position;

uniform mat4 models[100];

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
    mat4 unrotated_view;
};

flat out int instance;

void main()
{
	gl_Position = projection * view * models[gl_InstanceID] * vec4(position, 1.0f);
	instance = gl_InstanceID;
}