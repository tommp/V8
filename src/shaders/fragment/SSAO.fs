
//Based on the shader written by Martins Upitis (martinsh) (devlog-martinsh.blogspot.com)
//original technique is made by Arkano22 (www.gamedev.net/topic/550699-ssao-no-halo-artifacts/)

layout (location = 0) out float SSAO_buffer;

uniform sampler2D g_position;
uniform sampler2D g_albedo_spec;

layout (std140) uniform Light_data
{
	vec2 screen_size;
};
 
/* User variable defaults
int samples = 16; //ao sample count
float radius = 2.0; //ao radius
float power = 1.0;


bool optimize = true;
int minsamples = 2;
 

float aoclamp = 0.1; //depth clamp - reduces haloing at screen edges
float diffarea = 0.1; //self-shadowing reduction
float gdisplace = 0.06; //gauss bell center
float aowidth = 0.09; //gauss bell width
float lumInfluence = 0.8; //how much luminance affects occlusion
*/

int samples = 4;
float radius = 10.0;
float power = 2.0;
float slack = 0.001;
float depth_cutoff = 0.00000001;

float aoclamp = 10.0; //depth clamp - reduces haloing at screen edges
float aowidth = 0.09; //gauss bell width


const bool optimize = true;
int minsamples = 2;

 
//-------------------------------------------------------- 
 
void main(){
	vec2 frag_tex_coord = gl_FragCoord.xy / screen_size;
	vec4 frag_position = texture(g_position, frag_tex_coord);
	
	float depth = frag_position.a;

	float noiseX = ((fract(1.0 - frag_tex_coord.x * (screen_size.x/2.0)) * 0.25) + (fract(frag_tex_coord.y * (screen_size.y/2.0)) * 0.75)) * 2.0-1.0;
	float noiseY = ((fract(1.0 - frag_tex_coord.x * (screen_size.x/2.0)) * 0.75) + (fract(frag_tex_coord.y * (screen_size.y/2.0)) * 0.25)) * 2.0-1.0;

	vec2 noise =  vec2(noiseX,noiseY) * 2.0;

	float w = (1.0 / screen_size.x) / clamp(depth, aoclamp, 1.0) * noise.x;
	float h = (1.0 / screen_size.y) / clamp(depth, aoclamp, 1.0) * noise.y;

	if (optimize){
		vec3 color = texture(g_albedo_spec, frag_tex_coord).rgb;       
		vec3 lumcoeff = vec3(0.299, 0.587, 0.114);
		float lum = dot(color, lumcoeff);
		vec3 luminance = vec3(lum, lum, lum);
		samples = int(float(minsamples)+clamp(1.0-(pow(lum*1.0,4.0)),0.0,1.0)*(float(samples)-float(minsamples)));
	}
   	
   	float dz = 1.0/float(samples);
    float z = 1.0 - dz/2.0;
	float dl = 3.14159265 * (3.0 - sqrt(5.0));
	float lev = 0.0;

	float pw = 0;
	float ph = 0;
	float ao = 0;

	for (int i = 0; i <= samples; ++i){    
		float r = sqrt(1.0 - z);

		pw = cos(lev) * r * (1.0 - frag_position.a);
		ph = sin(lev) * r * (1.0 - frag_position.a);
	
		float dd = radius-depth;
		float temp = 0.0;
		float temp2 = 0.0;

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

		float range_check = step(depth_cutoff, radius / abs(depth - max(sample_depth, sample_depth2)));

		ao += range_check * sum;
		
		z = z - dz;
		lev = lev + dl;
	}
   
	ao /= samples + 0.1;
	ao = 1.0 - ao;

	SSAO_buffer =  pow(ao, power);	   
}