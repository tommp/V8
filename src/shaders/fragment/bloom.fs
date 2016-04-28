layout (location = 0) out vec4 bloom_buffer;

uniform sampler2D color;

in vec2 frag_tex_coord;

layout (std140) uniform Settings{
	vec4 shadow_settings;
	vec4 ppe_settings;
};

void main(){
	vec3 color = texture(color, frag_tex_coord).rgb;

	float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
	bloom_buffer = vec4(color * smoothstep(ppe_settings.x ,ppe_settings.y, brightness), 1.0);
}