
uniform sampler2D front_culled_map;
uniform sampler2D back_culled_map;

out vec2 map;

in vec2 frag_tex_coord;

layout (std140) uniform Plane_data
{
    vec2 plane_data;
};

float linearize_depth(float depth)
{	
    float linear_depth = (-2.0 * plane_data.x * plane_data.y) / (plane_data.y + plane_data.x - depth * (plane_data.y - plane_data.x)); 
    
    return linear_depth;
}

void main(){
	float back_sample = linearize_depth(texture(back_culled_map, frag_tex_coord).x);
	float front_sample = linearize_depth(texture(front_culled_map, frag_tex_coord).x);
	float thickness;
	if (back_sample != 0.0) {
		thickness = abs(back_sample - front_sample);
	}
	
	map = vec2(back_sample, thickness);
}