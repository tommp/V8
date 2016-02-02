#include "button_mappings.h"

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

SDL_Keycode Button_mappings::get_key(const Key& key)const{
	switch(key){
		case UP: return up;
		case DOWN: return down;
		case LEFT: return left;
		case RIGHT: return right;
		case JUMP: return jump;
		case INTERACT: return interact;
		case MENU: return menu;
		case A0: return a0;
		case A1: return a1;
		case A2: return a2;
		case A3: return a3;
		case QUIT: return quit;
		default: return menu;
	}
}