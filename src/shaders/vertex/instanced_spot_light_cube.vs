layout (location = 0) in vec3 position;

struct Spot_light {
	mat4 model_matrix;

	vec4 position;
	vec4 direction;
	vec4 color;
	vec4 color_components;
	
	float radius;
	float cut_off;
	float outer_cut_off;

	bool render_shadows;
	bool apply_ssao;

	float padding;
	float padding;
	float padding;
};

layout (std140) uniform Spot_lights{
	Spot_light lights[100];
	int num_lights;
};

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
    mat4 unrotated_view;
};

flat out int instance;

void main(){
	gl_Position = projection * view * lights[gl_InstanceID].model_matrix * vec4(position, 1.0f);
	instance = gl_InstanceID;
}