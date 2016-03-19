out vec4 color;

uniform sampler2D bloom;

layout (std140) uniform Light_data
{
	vec2 screen_size;
};

void main(){   
	vec2 frag_tex_coord = gl_FragCoord.xy / screen_size;
    
    vec3 hdrColor = texture(bloom, frag_tex_coord).rgb;      
    
    color = vec4(hdrColor, 1.0f);
}  