
uniform sampler2D discard_mask;
uniform bool use_mask;

out float view_depth;

in float frag_depth;

layout (std140) uniform Resolution_data{
	vec2 screen_size;
	vec2 resolution;
};

const float slack = 1.0;

void main(){
	if (use_mask) {
		float discard_depth = texture(discard_mask, gl_FragCoord.xy / resolution).x;

		if ((discard_depth == 0.0) || (frag_depth >= discard_depth - slack)) {
			discard;
		}
	}
	
	view_depth = frag_depth;
}