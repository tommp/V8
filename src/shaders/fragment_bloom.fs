out vec4 color;

uniform sampler2D bloom;

layout (std140) uniform Light_data
{
	vec2 screen_size;
};

void main(){   
	vec2 frag_tex_coord = gl_FragCoord.xy / screen_size;
    
    vec3 hdrColor = texture(bloom, frag_tex_coord).rgb;

    //HDR calculations
	const float gamma = 2.2;
	const float exposure = 1.0;
  
    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // Gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    color = vec4(mapped, 1.0);      
    
}  