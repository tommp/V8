#include "main.h"

int main(int argc, char** argv){

	/* INITIAL SETUP */
	/* ====================================== */

	/*Initializes SDL */
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		SDLerrorLogger("SDL initialization");
		std::cout<<"Failed to initialize SDL, see errorlog for details."<<std::endl;
		exit(EXIT_FAILURE);
	}

	/* Random seed to time */
    srand (time(NULL));
    /* ====================================== */

	/* MAIN VARS */
	/* ====================================== */
	Display display;

	Resource_manager resource_manager;

	Button_mappings button_mappings;

	State_handler state_handler(&button_mappings);

	World world(resource_manager, button_mappings);

    Timer cap_timer;
    Timer move_timer;
    /* ====================================== */

	/* Main loop */
	/*=======================================================*/
    while(state_handler.game_is_running()){

#if DISABLE_VSYNC
    	//Start cap timer
        cap_timer.restart();
#endif

	    /* Handle events in the queue */
	    state_handler.handle_events();

	    /* Update the position of all world objects */
	    float timedelta = move_timer.get_ticks() / 1000.f;
	    world.update_positions(timedelta);
	    move_timer.restart();

	    /* Detect collisions */
	    //world.detect_all_collisions();
	    
	    /* Resolve all collisions */
	    //cradlands.resolve_collisions();

	    /* Render to screen */
	    display.clear();
	    world.render_world();
	    display.present();

#if DISABLE_VSYNC
        /* If frame finished early */
        int frame_ticks = cap_timer.get_ticks();
        if( frame_ticks < SCREEN_TICKS_PER_FRAME )
        {
            /* Wait remaining time */
            SDL_Delay( SCREEN_TICKS_PER_FRAME - frame_ticks );
        }
#endif
	}
	/*=======================================================*/

	/* Store display setings */
	display.save_settings();
    /*Always be sure to clean up*/
    SDL_Quit();
    return 0;
}