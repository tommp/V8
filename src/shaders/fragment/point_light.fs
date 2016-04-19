struct Point_light {
	vec3 position;
	
	float radius;
	
	vec3 color;
	vec3 color_components;

	bool apply_ssao;
	bool render_shadows;
};

out vec4 color;

flat in int instance;
const int shininess = 32;
const int SHADOW_LAYERS = 1;
const float NUM_STEPS = 100.0;
const float OFFSET = 2.0;
const float STEPSIZE = 5.0;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;
uniform sampler2D SSAO_buffer;
uniform sampler2D shadow_layers[SHADOW_LAYERS];

uniform vec3 view_position;

uniform Point_light lights[100];

layout (std140) uniform Resolution_data{
	vec2 screen_size;
	vec2 resolution;
};

layout (std140) uniform Settings{
	vec4 shadow_settings;
};

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
    mat4 unrotated_view;
};

void apply_SSAO(inout vec3 ambient, vec2 sample_coords){
	float ambient_occlusion = texture(SSAO_buffer, sample_coords).r;
	ambient *= ambient_occlusion;
}

float calc_shadow_occlusion(vec3 light_direction, vec3 frag_position){
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

void main(){    
	vec2 frag_tex_coord = gl_FragCoord.xy / resolution;
	vec3 frag_position = texture(g_position, frag_tex_coord).xyz;
	vec3 normal = normalize(texture(g_normal, frag_tex_coord).xyz);
	vec3 sample_color = texture(g_albedo_spec, frag_tex_coord).rgb;
	
	float distance = length(lights[instance].position - frag_position);

	vec3 view_direction = normalize(view_position - frag_position);
	vec3 light_direction = normalize(lights[instance].position - frag_position);
	vec3 reflect_direction = reflect(-light_direction, normal);

	float diff = max(dot(normal, light_direction), 0.0);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);

	vec3 ambient = (lights[instance].color * lights[instance].color_components.x) * sample_color;

	vec3 diffuse = (lights[instance].color * lights[instance].color_components.y) * diff * sample_color;

	vec3 specular = (lights[instance].color * lights[instance].color_components.z) * spec * vec3(texture(g_albedo_spec, frag_tex_coord).a);

	float attenuation = clamp(1.0 - distance/lights[instance].radius, 0.0, 1.0);
	attenuation *= attenuation;
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	if (bool(shadow_settings.x)) {
		apply_SSAO(ambient, frag_tex_coord);
	}
	
	if (lights[instance].render_shadows && bool(shadow_settings.y)) {
		float shadow_occlusion = calc_shadow_occlusion(light_direction, frag_position);
		diffuse *= shadow_occlusion;
		specular *= shadow_occlusion;

	}

    color = vec4(ambient + diffuse + specular, 1.0);
}