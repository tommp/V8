
out vec4 color;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D ambient_color;
uniform sampler2D shadowed_color;

uniform vec3 light_position;
uniform float stepsize;
uniform float shadow_slack;
uniform float loop_offset;

layout (std140) uniform Light_data
{
	vec2 screen_size;
};

void main(){
	vec2 frag_tex_coord = gl_FragCoord.xy / screen_size;
	vec3 frag_position = texture(g_position, frag_tex_coord).xyz;
	vec3 normal = normalize(texture(g_normal, frag_tex_coord).xyz);

	float distance = length(light_position - frag_position);
	vec3 light_direction = normalize(light_position - frag_position);

	
	float shadow_occlusion = 1.0;
	float num_steps = distance / stepsize;
	float diff;

	vec3 trace_offset = light_direction * stepsize;
	vec3 trace_position = frag_position + trace_offset * loop_offset;
	vec3 sample_position = trace_position;

	for (float counter = loop_offset; counter < num_steps - loop_offset; counter += stepsize) {
		trace_position += trace_offset;

		//TODO::Move this out of here
		vec4 sample_offset = vec4(trace_position, 1.0);
        /*sample_offset = projection * sample_offset;
        sample_offset.xyz /= sample_offset.w;
        sample_offset.xyz = sample_offset.xyz * 0.5 + 0.5;*/

		sample_position = texture(g_position, sample_offset.xy).xyz;

		diff = length(sample_position - trace_position);

		shadow_occlusion -= (1 - step(shadow_slack, diff));
	}

	shadow_occlusion = clamp(shadow_occlusion, 0.0, 1.0);

	vec4 result = texture(ambient_color, frag_tex_coord);
	result += texture(shadowed_color, frag_tex_coord) * shadow_occlusion;
	color = result;
}