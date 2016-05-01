
uniform sampler2D discard_mask;
uniform bool use_mask;

out float view_depth;

in float frag_depth;

layout (std140) uniform Resolution_data{
	vec2 screen_size;
	vec2 resolution;
};

void main(){
	if (use_mask) {
		float discard_depth = texture(discard_mask, gl_FragCoord.xy / resolution).x;

		if ((frag_depth >= discard_depth)) {
			discard;
		}
	}
	
	view_depth = frag_depth;
}