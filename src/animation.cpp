#include "./headers/animation.h"

Frame::Frame(int x, int y, int w, int h, int frame_duration){
	duration = frame_duration;
	clip.x = x;
	clip.y = y;
	clip.w = w;
	clip.h = h;
}

void Animation::render_current(SDL_Renderer *ren, int x, int y){

	if( SDL_GetTicks() > current_frame_end ){
		if( current_frame == frames.end()) {
			current_frame = frames.begin();
		}
		else{
			current_frame++;
		}
		current_frame_end = SDL_GetTicks() + current_frame->duration;
	}

	texture->render(ren, x, y, &(current_frame->clip));
}