#include "./headers/events.h"

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

State_handler::State_handler(Button_mappings* map){
	mappings = map;
	game_running = true;
	menu_open = false;
	paused = false;
}

void State_handler::handle_events(){
	while( SDL_PollEvent( &ehandler ) != 0 )
	    {
	        /* User requests quit */
	        if( ehandler.type == SDL_QUIT ){
	            game_running = false;
	        }
	        else if( ehandler.type == SDL_KEYDOWN ){
	        	if (ehandler.key.keysym.scancode == mappings->quit) {
	        		game_running = false;
	        	}
	        }
	    }
}