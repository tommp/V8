
out vec4 color;

in vec2 frag_tex_coord;
flat in int instance;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D ambient_color;
uniform sampler2D shadowed_color;

uniform vec3 light_positions[100];
uniform float stepsizes[100];
uniform float shadow_slacks[100];
uniform float loop_offsets[100];

layout (std140) uniform Light_data
{
	vec2 screen_size;
	vec2 resolution;
};

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
    mat4 unrotated_view;
};

void main(){
	vec3 frag_position = texture(g_position, frag_tex_coord).xyz;
	vec3 normal = normalize(texture(g_normal, frag_tex_coord).xyz);

	float distance = length(light_positions[instance] - frag_position);
	vec3 light_direction = normalize(light_positions[instance] - frag_position);

	
	float shadow_occlusion = 1.0;
	float num_steps = distance / stepsizes[instance];
	float diff;

	vec3 trace_offset = light_direction * stepsizes[instance];
	vec3 trace_position = frag_position + trace_offset * loop_offsets[instance];
	vec3 sample_position = trace_position;

	for (float counter = loop_offsets[instance]; counter < num_steps - loop_offsets[instance]; counter += stepsizes[instance]) {
		trace_position += trace_offset;

		//TODO::Move this out of here
		vec4 sample_offset = vec4(trace_position, 1.0);
        sample_offset = projection * sample_offset;
        sample_offset.xyz /= sample_offset.w;
        sample_offset.xyz = sample_offset.xyz * 0.5 + 0.5;

		sample_position = texture(g_position, sample_offset.xy).xyz;

		diff = length(sample_position - trace_position);

		shadow_occlusion -= (1 - step(shadow_slacks[instance], diff));
	}

	shadow_occlusion = clamp(shadow_occlusion, 0.0, 1.0);

	vec4 result = texture(ambient_color, frag_tex_coord);
	result += texture(shadowed_color, frag_tex_coord) * shadow_occlusion;
	color = result;
}