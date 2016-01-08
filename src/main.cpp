#include "./headers/main.h"

int main(int argc, char** argv){

	/* INITIAL SETUP */
	/* ====================================== */

	/*Initializes SDL */
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		SDLerrorLogger("SDL initialization");
		std::cout<<"Failed to initialize SDL, see errorlog for details."<<std::endl;
		return 1;
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

    Timer cap_timer;
    Timer move_timer;
    /* ====================================== */


    /* TESTCODE */
    /*=======================================================*/
    
	resource_manager.load_texture(display.get_renderer(), "../pixelart/Animations/simple_animation.png", "simple");
   	resource_manager.load_texture(display.get_renderer(), "../pixelart/Foilage/SharpBush.png", "bush");
    Animation simplea(resource_manager, "simple", "../pixelart/Animations/simple_animation.txt");
    /* ===================================================== */

    Player number1(&simplea);

	/* Main loop */
	/*=======================================================*/
    while(state_handler.game_is_running()){

    	//Start cap timer
        cap_timer.start();

	    /* Handle events in the queue */
	    state_handler.handle_events();

	    float timedelta = move_timer.get_ticks() / 1000.f;

	    number1.update_position(timedelta);

	    move_timer.start();

	    /* Clear the screen */
	    display.clear();

	    number1.render_player(display.get_renderer());

	    /* Render and wait */
	    display.present();

        /* If frame finished early */
        int frame_ticks = cap_timer.get_ticks();
        if( frame_ticks < SCREEN_TICKS_PER_FRAME )
        {
            /* Wait remaining time */
            SDL_Delay( SCREEN_TICKS_PER_FRAME - frame_ticks );
        }
	}
	/*=======================================================*/

    /*Always be sure to clean up*/
    SDL_Quit();
    return 0;
}