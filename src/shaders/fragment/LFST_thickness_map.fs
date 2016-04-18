
uniform sampler2D front_culled_map;
uniform sampler2D back_culled_map;

out vec2 map;

in vec2 frag_tex_coord;

/*float linearize_depth(float depth)
{
    float z = depth * 2.0 - 1.0;
    float linear_depth = (2.0 * plane_data.x * plane_data.y) / (plane_data.y + plane_data.x - z * (plane_data.y - plane_data.x)); 

    linear_depth /= plane_data.y;
    
    return linear_depth;
}*/

void main(){
	float back_sample = texture(back_culled_map, frag_tex_coord).x;
	float thickness = 0.0;
	if (back_sample != 0.0) {
		thickness = abs(back_sample - texture(front_culled_map, frag_tex_coord).x);
	}
	
	map = vec2(back_sample, thickness);
}