#include "./headers/events.h"

Button_mappings::Button_mappings(){
	up = 'w';
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
	        	if (ehandler.key.keysym.sym == SDLK_ESCAPE) {
	        		game_running = false;
	        	}
	        }
	    }
}