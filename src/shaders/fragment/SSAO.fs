
out float SSAO_buffer;

in vec2 frag_tex_coord;

uniform sampler2D g_position;

layout (std140) uniform Resolution_data{
	vec2 screen_size;
	vec2 resolution;
};

layout (std140) uniform Plane_data
{
    vec2 plane_data;
};
 
const int SAMPLES = 1;
float radius = 1.0;
float power = 4.0;
float depth_exponent = 2.0; //Used to non-linearly kill AO at large depth differences
float slack = 0.00001; //Used to avoid self shadowing at flat planes, should be very small (but not 0).

void main(){
	float depth = texture(g_position, frag_tex_coord).w;

	float rand = fract(sin(dot(frag_tex_coord.xy ,vec2(12.9898,78.233))) * 43758.5453);
	vec2 step_dir = (normalize(vec2(rand)) * 2) / resolution;

	float ao = 0;

	for (int i = 0; i <= SAMPLES; ++i){    

		float coordw = frag_tex_coord.x + step_dir.x;
		float coordh = frag_tex_coord.y + step_dir.y;
		vec2 coord = vec2(coordw, coordh);

		float coordw2 = frag_tex_coord.x - step_dir.x;
		float coordh2 = frag_tex_coord.y - step_dir.y;
		vec2 coord2 = vec2(coordw2, coordh2);
		
	   	float sample_depth = texture(g_position, coord).a;
	   	float sample_depth2 = texture(g_position, coord2).a;
	 	
	 	float diff = depth - sample_depth - slack;
		float diff_sign = step(0.0, diff) * 2 - 1;
		float diff_mirror = depth - sample_depth2 - slack;
		float diff_mirror_sign = step(0.0, diff_mirror) * 2 - 1;
		float sum = clamp(-1 + diff_sign + diff_mirror_sign, 0.0, 1.0);

		float range_check = smoothstep(0.0, 1.0, radius - pow(abs(max(diff + slack, diff_mirror + slack)), depth_exponent));

		ao += range_check * sum;
	}
   
	ao /= SAMPLES + 0.1;
	ao = clamp(1.0 - ao, 0.0, 1.0);

	SSAO_buffer =  pow(ao, power);	   
}