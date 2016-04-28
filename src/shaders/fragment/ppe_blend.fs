out vec4 color;

layout (std140) uniform Resolution_data{
	vec2 screen_size;
	vec2 resolution;
};

in vec2 frag_tex_coord;

uniform sampler2D colors;

void main(){   
    vec3 sample_color = texture(colors, frag_tex_coord).rgb;
    
    color = vec4(sample_color, 1.0);
}  