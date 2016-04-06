
//Based on the shader written by Martins Upitis (martinsh) (devlog-martinsh.blogspot.com)
//original technique is made by Arkano22 (www.gamedev.net/topic/550699-ssao-no-halo-artifacts/)

layout (location = 0) out vec4 SSAO_buffer;

uniform sampler2D g_position;

layout (std140) uniform Light_data
{
	vec2 screen_size;
};
 
//------------------------------------------
//general stuff
 
//user variables

int samples = 32;
float power = 3.0;
float radius = 2.0;

float aoclamp = 0.1; //depth clamp - reduces haloing at screen edges
float diffarea = 0.1; //self-shadowing reduction
float gdisplace = 0.06; //gauss bell center
float aowidth = 0.09; //gauss bell width
float lumInfluence = 0.8; //how much luminance affects occlusion

const bool optimize = false;
int minsamples = 2;

const bool use_noise = false; //use noise instead of pattern for sample dithering
float noiseamount = 0.01; //dithering amount
 
const bool mist = false; //use mist?
float miststart = 2500.0; //mist start
float mistend = 3000.0; //mist end
 

 
//-------------------------------------------------------- 
 
void main(){
	vec2 frag_tex_coord = gl_FragCoord.xy / screen_size;
	vec4 frag_position = texture(g_position, frag_tex_coord);
	
	float depth = frag_position.z;

	float noiseX = ((fract(1.0 - frag_tex_coord.x * (screen_size.x/2.0)) * 0.25) + (fract(frag_tex_coord.y * (screen_size.y/2.0)) * 0.75)) * 2.0-1.0;
	float noiseY = ((fract(1.0 - frag_tex_coord.x * (screen_size.x/2.0)) * 0.75) + (fract(frag_tex_coord.y * (screen_size.y/2.0)) * 0.25)) * 2.0-1.0;
   
	if (use_noise){
		noiseX = clamp(fract(sin(dot(frag_tex_coord,vec2(12.9898,78.233))) * 43758.5453),0.0,1.0) * 2.0-1.0;
		noiseY = clamp(fract(sin(dot(frag_tex_coord,vec2(12.9898,78.233)*2.0)) * 43758.5453),0.0,1.0) * 2.0-1.0;
	}

	vec2 noise =  vec2(noiseX,noiseY) * 2.0;
	   
	float fog = clamp((frag_position.z - miststart) / mistend, 0.0, 1.0); 

	vec3 color = frag_position.rgb;       
	vec3 lumcoeff = vec3(0.299, 0.587, 0.114);
	float lum = dot(color.rgb, lumcoeff);
	vec3 luminance = vec3(lum, lum, lum);

	float w = (1.0 / screen_size.x) / clamp(depth, aoclamp, 1.0) * noise.x;
	float h = (1.0 / screen_size.y) / clamp(depth, aoclamp, 1.0) * noise.y;
   
	float pw;
	float ph;
   
	float ao;
	vec3 gi;

	//if (optimize)
	//{
	//	samples = int(float(minsamples)+clamp(1.0-(fog+pow(lum*1.0,4.0)),0.0,1.0)*(float(samples)-float(minsamples)));
	//}
   
	float dl = 3.14159265 * (3.0 - sqrt(5.0));
	float dz = 1.0/float(samples);
	float l = 0.0;
	float z = 1.0 - dz/2.0;


	for (int i = 0; i <= samples; i ++){    
		float r = sqrt(1.0 - z);
	
		float dd = radius-depth;
		float temp = 0.0;
		float temp2 = 0.0;

		float coordw = frag_tex_coord.x + pw * w * dd;
		float coordh = frag_tex_coord.y + ph * h * dd;
		vec2 coord = vec2(coordw, coordh);

		float coordw2 = frag_tex_coord.x - pw * w * dd;
		float coordh2 = frag_tex_coord.y - ph * h * dd;
		vec2 coord2 = vec2(coordw2, coordh2);
	   
		pw = cos(l) * r * (1.0 - clamp((frag_position.z - miststart) / mistend, 0.0, 1.0));
		ph = sin(l) * r * (1.0 - clamp((frag_position.z - miststart) / mistend, 0.0, 1.0));

		vec3 bleed = vec3(0.0, 0.0, 0.0);
		
	   	float sample_depth = texture(g_position, coord).z;
	   	float sample_depth2 = texture(g_position, coord2).z;
		int far = 0;
	 
		float garea = aowidth;
		float diff = (depth - sample_depth) * 100.0; //depth difference (0-100) TODO THIS GOES TO HELL

		if (diff < gdisplace){
			garea = diffarea;
		}
		else{
			far = 1;
		}
	   
		float gauss = pow(2.7182,-2.0) * (diff - gdisplace) * (diff - gdisplace) / (garea * garea);
		temp = gauss;
		
		if (far > 0){
			float garea = aowidth;
			float diff = (sample_depth2 - depth) * 100.0;

			if (diff < gdisplace){
				garea = diffarea;
			}
			else{
				far = 1;
			}
		   
			float gauss = pow(2.7182,-2.0) * (diff - gdisplace) * (diff - gdisplace) / (garea * garea);
			temp2 = gauss;

			temp += (1.0 - temp) * temp2;
			bleed = texture(g_position, coord).rgb;
		}

		ao += temp;
		gi += temp*bleed;
		
		z = z - dz;
		l = l + dl;
	}
   
	gi /= samples + 0.1;
   
	ao /= samples + 0.1;
	ao = 1.0 - ao;
   
	if (mist){
		ao = mix(ao, 1.0, fog);
	}

	float final = ao;

	SSAO_buffer =  vec4(vec3(pow(final, power)), 1.0);	   
}