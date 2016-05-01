
uniform sampler2D front_culled_map;
uniform sampler2D back_culled_map;

out vec2 map;

in vec2 frag_tex_coord;

void main(){
	float front_sample = texture(back_culled_map, frag_tex_coord).x;
	float thickness = 0.0;

	thickness = abs(abs(front_sample) - abs(texture(front_culled_map, frag_tex_coord).x));
	
	map = vec2(front_sample, thickness);
}