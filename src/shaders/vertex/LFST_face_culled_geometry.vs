layout (location = 0) in vec3 position;

out float frag_depth;

uniform mat4 models[100];

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
    mat4 unrotated_view;
};

void main(){
	vec4 view_pos = models[gl_InstanceID] * vec4(position, 1.0f);

    frag_depth = view_pos.z; 

    gl_Position = projection * view_pos;
}