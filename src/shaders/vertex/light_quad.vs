layout (location = 0) in vec3 position;

uniform mat4 model;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
    mat4 unrotated_view;
};

void main(){
	gl_Position = projection * model * vec4(position, 1.0f);
}