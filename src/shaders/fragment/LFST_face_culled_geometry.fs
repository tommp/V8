
uniform sampler2D float discard_mask;

out float view_depth;

in float frag_depth;

void main(){
	if (frag_depth >= texture(discard_mask, gl_Frag_Coord.xy / resolution)) {
		discard;
	}

	view_depth = frag_depth;
}