layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

out vec2 frag_tex_coord;
out vec3 frag_position;
out vec3 frag_normal;

uniform mat4 models[100];
uniform mat3 normal_models[100];

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
    mat4 unrotated_view;
};

void main(){
	vec4 view_pos = models[gl_InstanceID] * vec4(position, 1.0f);

    frag_position = view_pos.xyz; 

    gl_Position = projection * view_pos;

    frag_tex_coord = tex_coord;

    frag_normal = normal_models[gl_InstanceID] * normal;
}