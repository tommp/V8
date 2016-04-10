struct Point_light {
	vec3 position;
	
	float radius;
	
	vec3 color;
	vec3 color_components;

	bool render_shadows;
	float stepsize;
	float shadow_slack;
	float loop_offset;
};

out vec4 color;

flat in int instance;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_spec;

uniform vec3 view_position;

uniform Point_light lights[100];

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

void render_shadows(float distance, 
					vec3 light_direction, 
					vec3 frag_position, 
					inout vec3 diffuse, 
					inout vec3 specular){
	float shadow_occlusion = 1.0;
	float num_steps = distance / lights[instance].stepsize;
	float diff;

	vec3 trace_offset = light_direction * lights[instance].stepsize;
	vec3 trace_position = frag_position + trace_offset * lights[instance].loop_offset;

	vec3 sample_position = trace_position;

	vec4 sample_offset = vec4(trace_offset, 1.0);
	sample_offset = projection * sample_offset;

	vec4 sample_coords = vec4(trace_position, 1.0);
    sample_coords = projection * sample_coords;

    vec3 final_coords;

	for (float counter = lights[instance].loop_offset; counter < num_steps - lights[instance].loop_offset; counter += lights[instance].stepsize) {
		trace_position += trace_offset;
		sample_coords += sample_offset;

		final_coords = sample_coords.xyz / sample_coords.w;
		final_coords = final_coords * 0.5 + 0.5;

		sample_position = texture(g_position, final_coords.xy).xyz;

		diff = length(sample_position - trace_position);

		shadow_occlusion -= (1 - step(lights[instance].shadow_slack, diff));
	}

	shadow_occlusion = clamp(shadow_occlusion, 0.0, 1.0);

	diffuse *= shadow_occlusion;
	specular *= shadow_occlusion;
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

	
	if (lights[instance].render_shadows) {
		render_shadows(distance, light_direction, frag_position, diffuse, specular);
	}

    color = vec4(ambient + diffuse + specular, 1.0);
}