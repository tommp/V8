#include "./headers/frame.h"

Frame::Frame(int x, int y, int w, int h, int frame_duration){
	duration = frame_duration;
	clip.x = x;
	clip.y = y;
	clip.w = w;
	clip.h = h;
}