layout (location = 0) out float SSAO_buffer;
in vec2 frag_tex_coords;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D SSAO_noise_buffer;

const int KERNELSIZE = 32;
const int NOISESIZE = 4;
float radius = 3.0;
float power = 3.0;

layout (std140) uniform SSAO_kernel
{
	uniform vec3 samples[KERNELSIZE];
};

layout (std140) uniform Light_data
{
	vec2 screen_size;
};

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
    mat4 unrotated_view;
};

vec2 noise_scale = vec2(screen_size.x/NOISESIZE, screen_size.y/NOISESIZE);

void main(){
    vec2 frag_tex_coord = gl_FragCoord.xy / screen_size;
    vec3 frag_position = texture(g_position, frag_tex_coord).xyz;
	vec3 normal = texture(g_normal, frag_tex_coord).rgb;
	vec3 random_vec = texture(SSAO_noise_buffer, frag_tex_coord * noise_scale).xyz; 

	vec3 tangent = normalize(random_vec - normal * dot(random_vec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal); 

	float occlusion = 0.0;
    for(int i = 0; i < KERNELSIZE; ++i){
        vec3 sample = TBN * samples[i]; 
        sample = frag_position + sample * radius; 
        
        vec4 offset = vec4(sample, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sample_depth = -texture(g_position, offset.xy).a;
        
        float range_check = smoothstep(0.0, 1.0, radius / abs(frag_position.z - sample_depth));
        occlusion += (sample_depth >= sample.z ? 1.0 : 0.0) * range_check;           
    }

    occlusion = 1.0 - (occlusion / KERNELSIZE);
    
    SSAO_buffer =  pow(occlusion, power);
}