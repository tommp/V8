/*
Based on the shader written by Martins Upitis (martinsh) (devlog-martinsh.blogspot.com)
original technique is made by Arkano22 (www.gamedev.net/topic/550699-ssao-no-halo-artifacts/)
*/
layout (location = 0) out vec4 SSAO_buffer;

uniform sampler2D g_position;

layout (std140) uniform Light_data
{
	vec2 screen_size;
};

layout (std140) uniform Plane_data
{
	float near_plane;
	float far_plane;
};

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
    mat4 unrotated_view;
};
 
#define PI    3.14159265
 
float width = screen_size.x;
float height = screen_size.y;
 
vec2 texCoord = gl_FragCoord.xy / screen_size;
 
//------------------------------------------
//general stuff
 
//user variables
int samples = 32; //ao sample count
bool optimize = false;
int minsamples = 2;
 
float radius = 2.0; //ao radius
float aoclamp = 0.1; //depth clamp - reduces haloing at screen edges
bool noise = true; //use noise instead of pattern for sample dithering
float noiseamount = 0.01; //dithering amount
 
float diffarea = 0.1; //self-shadowing reduction
float gdisplace = 0.06; //gauss bell center
float aowidth = 0.09; //gauss bell width
 
bool mist = false; //use mist?
float miststart = 0.0; //mist start
float mistend = 100.0; //mist end
 
float lumInfluence = 0.8; //how much luminance affects occlusion
 
//--------------------------------------------------------
 
 
float doMist(){
		float zdepth = (view * texture(g_position, texCoord)).a;
		float depth = -far_plane * near_plane / (zdepth * (far_plane - near_plane) - far_plane);
		return clamp((depth - miststart) / mistend, 0.0, 1.0);
}
 
vec2 rand(vec2 coord){
   
		float noiseX = ((fract(1.0 - coord.s * (width/2.0)) * 0.25) + (fract(coord.t * (height/2.0)) * 0.75)) * 2.0-1.0;
		float noiseY = ((fract(1.0 - coord.s * (width/2.0)) * 0.75) + (fract(coord.t * (height/2.0)) * 0.25)) * 2.0-1.0;
	   
		if (noise)
		{
				noiseX = clamp(fract(sin(dot(coord,vec2(12.9898,78.233))) * 43758.5453),0.0,1.0) * 2.0-1.0;
				noiseY = clamp(fract(sin(dot(coord,vec2(12.9898,78.233)*2.0)) * 43758.5453),0.0,1.0) * 2.0-1.0;
		}
		return vec2(noiseX,noiseY) * 2.0;
}
 
vec3 readColor(in vec2 coord)  {  
	 return ( view * texture(g_position, coord)).rgb;  
}
   
float readDepth(in vec2 coord){
		coord.x = clamp(coord.x,0.001,0.999);
        coord.y = clamp(coord.y,0.001,0.999);
        return (2.0 * near_plane) / (far_plane + near_plane - (view * texture2D(g_position, coord)).a * (far_plane-near_plane));
}
 
float compareDepths(in float depth1, in float depth2, inout int far){  
	float garea = aowidth; //gauss bell width    
	float diff = (depth1 - depth2)*100.0; //depth difference (0-100)
	//reduce left bell width to avoid self-shadowing
	if (diff < gdisplace){
		garea = diffarea;
	}
	else{
		far = 1;
	}
   
	float gauss = pow(2.7182,-2.0 * (diff - gdisplace) * (diff - gdisplace) / (garea * garea));
	return gauss;
}  
 
vec3 calAO(float depth, float dw, float dh, inout float ao)
{  
	vec3 bleed = vec3(0.0, 0.0, 0.0);
	float dd = radius-depth;
	float temp = 0.0;
	float temp2 = 0.0;
	float coordw = texCoord.x + dw*dd;
	float coordh = texCoord.y + dh*dd;
	float coordw2 = texCoord.x - dw*dd;
	float coordh2 = texCoord.y - dh*dd;
   
	vec2 coord = vec2(coordw, coordh);
	vec2 coord2 = vec2(coordw2, coordh2);
   
	int far = 0;
	temp = compareDepths(depth, readDepth(coord), far);
	//DEPTH EXTRAPOLATION:
	if (far > 0)
	{
		temp2 = compareDepths(readDepth(coord2), depth, far);
		temp += (1.0 - temp) * temp2;
		bleed = readColor(coord);
	}
	ao += temp;
	return temp*bleed;
}
 
void main(void)
{
	vec2 noise = rand(texCoord);
	float depth = readDepth(texCoord);
   
	float fog = doMist();

	vec3 color = (view * texture(g_position, texCoord)).rgb;       
	vec3 lumcoeff = vec3(0.299, 0.587, 0.114);
	float lum = dot(color.rgb, lumcoeff);
	vec3 luminance = vec3(lum, lum, lum);

	float w = (1.0 / width) / clamp(depth, aoclamp, 1.0) * noise.x;
	float h = (1.0 / height) / clamp(depth, aoclamp, 1.0) * noise.y;
   
	float pw;
	float ph;
   
	float ao;
	vec3 gi;

	//if (optimize)
	//{
	//	samples = int(float(minsamples)+clamp(1.0-(fog+pow(lum*1.0,4.0)),0.0,1.0)*(float(samples)-float(minsamples)));
	//}
   
	float dl = PI * (3.0 - sqrt(5.0));
	float dz = 1.0/float(samples);
	float l = 0.0;
	float z = 1.0 - dz/2.0;


	for (int i = 0; i <= samples; i ++)
	{    
		float r = sqrt(1.0 - z);
	   
		pw = cos(l) * r * (1.0 - doMist());
		ph = sin(l) * r * (1.0 - doMist());
		gi += calAO(depth, pw * w, ph * h, ao);        
		z = z - dz;
		l = l + dl;
	}
   
	gi /= samples + 0.1;
   
	ao /= samples + 0.1;
	ao = clamp(0.0, 1.0, 1.0 - ao);
   
	//if (mist){
	//	ao = mix(ao, 1.0, fog);
	//}

	vec3 final = vec3(mix(vec3(ao),vec3(1.0),luminance*lumInfluence)); //ambient occlusion only
	SSAO_buffer = vec4(vec3(final),1.0);
	   
}