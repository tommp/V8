struct Directional_light {
	vec4 direction;
	vec4 color;
	vec4 color_components;

	bool render_shadows;
	bool apply_ssao;

	vec2 padding;
};

struct Point_light {
	mat4 model_matrix;

	vec4 position;
	vec4 color;
	vec4 color_components;

	float radius;
	bool render_shadows;
	bool apply_ssao;

	float padding;	
};

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

uniform sampler2D g_position;
uniform sampler2D g_albedo_spec;
uniform sampler2D shadow_layers[SHADOW_LAYERS];

layout (std140) uniform Dir_lights{
	Directional_light lights[10];
	int num_lights;
};

layout (std140) uniform Point_lights{
	Point_light lights[100];
	int num_lights;
};

layout (std140) uniform Spot_lights{
	Spot_light lights[100];
	int num_lights;
};


void main(){
	vec3 total_lighting = vec3(0.0, 0.0, 0.0);
	float shadow_occlusion = 0.0;

	vec3 trace_offset = light_direction * STEPSIZE;
	vec3 trace_position = frag_position + trace_offset * OFFSET;

	vec4 sample_offset = vec4(trace_offset, 1.0);
    vec4 sample_coords = projection * vec4(trace_position, 1.0);

    vec3 final_coords;
    vec2 layer_sample;

	for (float counter = OFFSET; counter < NUM_STEPS; counter += STEPSIZE) {
		trace_position += trace_offset;
		sample_coords += sample_offset;

		final_coords = (sample_coords.xyz / sample_coords.w) * 0.5 + 0.5;

		// UNROLLED LOOP 
		layer_sample = texture(shadow_layers[0], final_coords.xy).xy;
		float sign_1 = step(0.0, trace_position.z - (layer_sample.x - layer_sample.y)) * 2 - 1;
		float sign_2 = step(0.0, layer_sample.x - trace_position.z) * 2 - 1;
		shadow_occlusion = max(sign_1 + sign_2, shadow_occlusion);
		// ====================
	}

	shadow_occlusion /= 2;
	shadow_occlusion = 1 - step(1.0, shadow_occlusion); 

	return shadow_occlusion;
}