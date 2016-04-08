
out float SSAO_buffer;

in vec2 frag_tex_coord;

uniform sampler2D g_position;

layout (std140) uniform Light_data{
	vec2 screen_size;
	vec2 resolution;
};

layout (std140) uniform Plane_data
{
    vec2 plane_data;
};
 
int samples = 8;
float radius = 2.0;
float power = 4.0;
float depth_exponent = 2.0; //Used to non-linearly kill AO at large depth differences
float slack = 0.00001; //Used to avoid self shadowing at flat planes, should be very small (but not 0).

void main(){
	float depth = texture(g_position, frag_tex_coord).w;

	float w = (1.0 / resolution.x);
	float h = (1.0 / resolution.y);

   	float dz = 1.0/float(samples);
    float z = 1.0 - dz/2.0;
	float dl = 3.14159265 * (3.0 - sqrt(5.0));
	float lev = 0.0;

	float pw = 0;
	float ph = 0;
	float ao = 0;

	for (int i = 0; i <= samples; ++i){    
		float r = sqrt(1.0 - z);

		//TODO:: Gives sinusoidal artifacts at straight occluded edges, find alternative?
		pw = sin(lev) * r * (1.0 - depth);
		ph = cos(lev) * r * (1.0 - depth);
	
		float dd = radius-depth;

		float coordw = frag_tex_coord.x + pw * w * dd;
		float coordh = frag_tex_coord.y + ph * h * dd;
		vec2 coord = vec2(coordw, coordh);

		float coordw2 = frag_tex_coord.x - pw * w * dd;
		float coordh2 = frag_tex_coord.y - ph * h * dd;
		vec2 coord2 = vec2(coordw2, coordh2);

		
	   	float sample_depth = texture(g_position, coord).a;
	   	float sample_depth2 = texture(g_position, coord2).a;
	 
		float diff = depth - (sample_depth  + slack);
		float diff_sign = sign(diff);
		float diff_mirror = depth - (sample_depth2 + slack);
		float diff_mirror_sign = sign(diff_mirror);
		float sum = clamp(-1 + diff_sign + diff_mirror_sign, 0.0, 1.0);

		float range_check = smoothstep(0.0, 1.0, radius - pow(abs(max(diff + slack, diff_mirror + slack)), depth_exponent));

		ao += range_check * sum;
		
		z = z - dz;
		lev = lev + dl;
	}
   
	ao /= samples + 0.1;
	ao = 1.0 - ao;

	SSAO_buffer =  pow(ao, power);	   
}