struct Directional_light {
	vec3 direction;
	
	vec3 color;
	vec3 color_components;

	bool render_shadows;
	float stepsize;
	float shadow_slack;
	float loop_offset;
	float num_steps;
};

out vec4 color;

flat in int instance;
const int shininess = 32;
const int SHADOW_LAYERS = 3;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;
uniform sampler2D SSAO_buffer;
uniform sampler2D shadow_layers[SHADOW_LAYERS];

uniform vec3 view_position;

uniform Directional_light lights[100];

layout (std140) uniform Resolution_data{
	vec2 screen_size;
	vec2 resolution;
};

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
    mat4 unrotated_view;
};


void render_shadows(vec3 frag_position, 
					inout vec3 diffuse, 
					inout vec3 specular){
	float shadow_occlusion = 1.0;

	vec3 light_direction = normalize(-lights[instance].direction);

	vec3 trace_offset = light_direction * lights[instance].stepsize;
	vec3 trace_position = frag_position + trace_offset * lights[instance].loop_offset;

	vec4 sample_offset = vec4(trace_offset, 1.0);
	vec4 sample_coords = vec4(trace_position, 1.0);

	sample_offset = projection * sample_offset;
    sample_coords = projection * sample_coords;

    vec3 final_coords;
    vec2 layer_sample;

	for (float counter = lights[instance].loop_offset; counter < lights[instance].num_steps - lights[instance].loop_offset; counter += lights[instance].stepsize) {
		trace_position += trace_offset;
		sample_coords += sample_offset;

		final_coords = sample_coords.xyz / sample_coords.w;
		final_coords = final_coords * 0.5 + 0.5;

		/* UNROLLED LOOP */
		layer_sample = texture(shadow_layers[0], final_coords.xy).xy;

		if (trace_position.z > (layer_sample.x - layer_sample.y) && trace_position.z < layer_sample.x) {
			shadow_occlusion = 0;
		}

		layer_sample = texture(shadow_layers[1], final_coords.xy).xy;

		if (trace_position.z > (layer_sample.x - layer_sample.y) && trace_position.z < layer_sample.x) {
			shadow_occlusion = 0;
		}
		/* ==================== */

	}

	diffuse *= shadow_occlusion;
	specular *= shadow_occlusion;
}

void main(){
	vec2 frag_tex_coord = (gl_FragCoord.xy / resolution);
	vec3 frag_position = texture(g_position, frag_tex_coord).rgb;
	vec3 normal = normalize(texture(g_normal, frag_tex_coord).rgb);
	vec3 sample_color = texture(g_albedo_spec, frag_tex_coord).rgb;

	float ambient_occlusion = texture(SSAO_buffer, frag_tex_coord).r;

	vec3 view_direction = normalize(view_position - frag_position);
	vec3 light_direction = normalize(-lights[instance].direction);
	vec3 reflect_direction = reflect(-light_direction, normal);

	float diff = max(dot(normal, light_direction), 0.0);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);

	vec3 ambient = (lights[instance].color * lights[instance].color_components.x) * sample_color;

	ambient *= ambient_occlusion;

	vec3 diffuse = (lights[instance].color * lights[instance].color_components.y) * diff * sample_color;

	vec3 specular = (lights[instance].color * lights[instance].color_components.z) * spec * vec3(texture(g_albedo_spec, frag_tex_coord).a);
  
	if (lights[instance].render_shadows) {
		render_shadows(frag_position, diffuse, specular);
	}

    color = vec4(ambient + diffuse + specular, 1.0);
}