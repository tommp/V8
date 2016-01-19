#include "./headers/button_mappings.h"

Button_mappings::Button_mappings(){
	up = SDL_SCANCODE_W;
	down = SDL_SCANCODE_S;
	left = SDL_SCANCODE_A;
	right = SDL_SCANCODE_D;
	jump = SDL_SCANCODE_SPACE;
	interact = SDL_SCANCODE_E;
	menu = SDL_SCANCODE_M;
	a0 = SDL_SCANCODE_H;
	a1 = SDL_SCANCODE_J;
	a2 = SDL_SCANCODE_K;
	a3 = SDL_SCANCODE_L;

	quit = SDL_SCANCODE_ESCAPE;
}