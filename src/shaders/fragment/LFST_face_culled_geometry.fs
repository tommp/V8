
uniform sampler2D float discard_mask;
uniform bool use_mask;

out float view_depth;

in float frag_depth;

void main(){
	if (use_mask) {
		if (frag_depth >= texture(discard_mask, gl_Frag_Coord.xy / resolution)) {
			//Write here too
			discard;
		}
	}
	
	view_depth = frag_depth;
}