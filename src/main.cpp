#include "main.h"

int main(int argc, char** argv){

	/* INITIAL SETUP */
	/* ====================================== */

	/*Initializes SDL */
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "Failed to initialize SDL, see errorlog for details."<<std::endl;
		SDLerrorLogger("SDL initialization");
		exit(EXIT_FAILURE);
	}

	/* Random seed to time */
	srand (time(NULL));

	std::cout << "Converting images..." << std::endl;
	convert_all_images();
	std::cout << "Images converted!" << std::endl;
	/* ====================================== */

	/* MAIN VARS */
	/* ====================================== */
	Resource_manager resource_manager;

	Renderer renderer(resource_manager);

	State_handler state_handler(resource_manager);

	World world(resource_manager);

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
		if (!world.update_positions(timedelta, renderer)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to update world positions!" << std::endl;
			errorlogger("ERROR: Failed to update world positions!");
			exit(EXIT_FAILURE);
		}
		move_timer.restart();

		/* Resolve all collisions */
		if (!world.resolve_collisions()){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to resolve world collisions!" << std::endl;
			errorlogger("ERROR: Failed to resolve world collisions!");
			exit(EXIT_FAILURE);
		}

		/* Render to screen */
		world.render_world(renderer);
		if(check_ogl_error()){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render world!" << std::endl;
			errorlogger("ERROR: Failed to render world!");
			exit(EXIT_FAILURE);
		}

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
	//display.save_settings();
	/*Always be sure to clean up*/
	SDL_Quit();
	return 0;
}