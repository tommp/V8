
uniform sampler2D float front_culled_map;
uniform sampler2D float back_culled_map;

out vec2 map;
out float offset;

in vec2 frag_tex_coord;

void main(){
	map =  = vec2(front_sample - texture(back_culled_map, frag_tex_coord), front_sample);
}