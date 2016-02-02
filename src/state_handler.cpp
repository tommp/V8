#include "state_handler.h"

State_handler::State_handler(const Resource_manager& init_manager){
	manager = &init_manager;
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
	        	if (ehandler.key.keysym.scancode == manager->get_button_map_key("player", QUIT)) {
	        		game_running = false;
	        	}
	        }
	    }
}