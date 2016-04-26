
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
float radius = 2.0;
float power = 1.0;
float depth_culler = 3000.0;
float slack = 0.00001; //Used to avoid self shadowing at flat planes, should be very small (but not 0).

void main(){
	float depth = texture(g_position, frag_tex_coord).w;

	float rand_x = 1.0;
	float rand_y = 1.0;
	vec2 step_dir = (normalize(vec2(rand_x, rand_y))) / resolution;
	step_dir *= radius;

	float ao = 0;

	for (int i = 0; i <= SAMPLES; ++i){    

		vec2 coord_E  = vec2(frag_tex_coord.x + step_dir.x, frag_tex_coord.y);
		vec2 coord_W  = vec2(frag_tex_coord.x - step_dir.x, frag_tex_coord.y);

		vec2 coord_NE = vec2(frag_tex_coord.x + step_dir.x, frag_tex_coord.y + step_dir.y);
		vec2 coord_SW = vec2(frag_tex_coord.x - step_dir.x, frag_tex_coord.y - step_dir.y);

		vec2 coord_N = vec2(frag_tex_coord.x, frag_tex_coord.y + step_dir.y);
		vec2 coord_S = vec2(frag_tex_coord.x, frag_tex_coord.y - step_dir.y);

		vec2 coord_NW = vec2(frag_tex_coord.x - step_dir.x, frag_tex_coord.y + step_dir.y);
		vec2 coord_SE = vec2(frag_tex_coord.x + step_dir.x, frag_tex_coord.y - step_dir.y);
		
	   	float sample_depth_NE = texture(g_position, coord_NE).a;
	   	float sample_depth_SW = texture(g_position, coord_SW).a;
	   	float sample_depth_NW = texture(g_position, coord_NW).a;
	   	float sample_depth_SE = texture(g_position, coord_SE).a;
	   	float sample_depth_N = texture(g_position, coord_N).a;
	   	float sample_depth_S = texture(g_position, coord_S).a;
	   	float sample_depth_W = texture(g_position, coord_W).a;
	   	float sample_depth_E = texture(g_position, coord_E).a;
	 	
	 	float diff_NE = step(0.0, depth - sample_depth_NE - slack) * 2 - 1;
		float diff_SW = step(0.0, depth - sample_depth_SW - slack) * 2 - 1;
		float sum = clamp(-1 + diff_NE + diff_SW, 0.0, 1.0);

		float diff_NW = step(0.0, depth - sample_depth_NW - slack) * 2 - 1;
		float diff_SE = step(0.0, depth - sample_depth_SE - slack) * 2 - 1;
		sum += clamp(-1 + diff_NW + diff_SE, 0.0, 1.0);

		float diff_N = step(0.0, depth - sample_depth_N - slack) * 2 - 1;
		float diff_S = step(0.0, depth - sample_depth_S - slack) * 2 - 1;
		sum += clamp(-1 + diff_N + diff_S, 0.0, 1.0);

		float diff_W = step(0.0, depth - sample_depth_W - slack) * 2 - 1;
		float diff_E = step(0.0, depth - sample_depth_E - slack) * 2 - 1;
		sum += clamp(-1 + diff_W + diff_E, 0.0, 1.0);

		float avg_diff = depth - ((sample_depth_NE + sample_depth_SW + sample_depth_NW + sample_depth_SE + sample_depth_N + sample_depth_S + sample_depth_W + sample_depth_E) / 8);

		float depth_check = smoothstep(0.0, 1.0, radius - depth_culler * abs(avg_diff));

		ao += depth_check * (sum/4);
	}
   
	ao /= SAMPLES;
	ao = clamp(1.0 - ao, 0.0, 1.0);

	SSAO_buffer =  pow(ao, power);	   
}