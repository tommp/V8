out vec4 color;

layout (std140) uniform Resolution_data{
	vec2 screen_size;
	vec2 resolution;
};


in vec2 frag_tex_coord;
uniform sampler2D colors;

layout (std140) uniform Settings{
	vec4 shadow_settings;
	vec4 ppe_settings;
};

void main(){   
    vec3 hdr_color = texture(colors, frag_tex_coord).rgb;
  
    vec3 mapped = vec3(1.0) - exp(-hdr_color * ppe_settings.w);
    mapped = pow(mapped, vec3(1.0 / ppe_settings.z));
    
    color = vec4(mapped, 1.0);
}  