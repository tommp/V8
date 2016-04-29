layout (location = 0) in vec3 position;

struct Point_light {
	mat4 model_matrix;

	vec4 position;
	vec4 color;

	float radius;
	bool render_shadows;
	bool apply_ssao;

	float padding;	
};

layout (std140) uniform Point_lights{
	Point_light lights[100];
	int num_lights;
};

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
    mat4 unrotated_view;
};

flat out int instance;

void main(){
	gl_Position = projection * lights[gl_InstanceID].model_matrix * vec4(position, 1.0f);
	instance = gl_InstanceID;
}