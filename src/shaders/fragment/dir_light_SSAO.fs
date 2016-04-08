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

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;
uniform sampler2D SSAO_buffer;

uniform vec3 view_position;

uniform Directional_light light;

layout (std140) uniform Light_data{
	vec2 screen_size;
	vec2 resolution;
};

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
    mat4 unrotated_view;
};

const int shininess = 32;

void render_shadows(vec3 frag_position, 
					inout vec3 diffuse, 
					inout vec3 specular){
	float shadow_occlusion = 1.0;
	float diff;

	vec3 light_direction = normalize(-light.direction);

	vec3 trace_offset = light_direction * light.stepsize;
	vec3 trace_position = frag_position + trace_offset * light.loop_offset;

	vec3 sample_position = trace_position;

	vec4 sample_offset = vec4(trace_offset, 1.0);
	sample_offset = projection * sample_offset;

	vec4 sample_coords = vec4(trace_position, 1.0);
    sample_coords = projection * sample_coords;

    vec3 final_coords;

	for (float counter = light.loop_offset; counter < light.num_steps - light.loop_offset; counter += light.stepsize) {
		trace_position += trace_offset;
		sample_coords += sample_offset;

		final_coords = sample_coords.xyz / sample_coords.w;
		final_coords = final_coords * 0.5 + 0.5;

		sample_position = texture(g_position, final_coords.xy).xyz;

		diff = length(sample_position - trace_position);

		shadow_occlusion -= (1 - step(light.shadow_slack, diff));
	}

	shadow_occlusion = clamp(shadow_occlusion, 0.0, 1.0);

	diffuse *= shadow_occlusion;
	specular *= shadow_occlusion;
}

void main(){
	vec2 frag_tex_coord = (gl_FragCoord.xy / resolution);
	vec3 normal = normalize(texture(g_normal, frag_tex_coord).rgb);
	vec3 view_direction = normalize(view_position - texture(g_position, frag_tex_coord).rgb);
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	vec3 frag_position = texture(g_position, frag_tex_coord).rgb;
	float ambient_occlusion = texture(SSAO_buffer, frag_tex_coord).r;

	vec3 light_direction = normalize(-light.direction);

	float diff = max(dot(normal, light_direction), 0.0);

	vec3 reflect_direction = reflect(-light_direction, normal);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);

	vec3 ambient = (light.color * light.color_components.x) * vec3(texture(g_albedo_spec, frag_tex_coord).rgb);

	ambient *= ambient_occlusion;

	vec3 diffuse = (light.color * light.color_components.y) * diff * vec3(texture(g_albedo_spec, frag_tex_coord).rgb);

	vec3 specular = (light.color * light.color_components.z) * spec * vec3(texture(g_albedo_spec, frag_tex_coord).a);
  
	if (light.render_shadows) {
		render_shadows(frag_position, diffuse, specular);
	}

    color = vec4(ambient + diffuse + specular, 1.0);
}