#include "main.h"

int main(int argc, char** argv){

	/* INITIAL SETUP */
	/* ====================================== */
	std::cout <<"\n\n____   __________  " << std::endl;
	std::cout << "\\   \\ /   /  __  \\ " << std::endl;
	std::cout << " \\   Y   />      < " << std::endl;
	std::cout << "  \\     //   --   \\" << std::endl;
	std::cout << "   \\___/ \\______  /" << std::endl;
	std::cout << "                \\/" << std::endl;
	std::cout << "\nV8 Engine version xx \n" << std::endl;
	std::cout << "Performing initial setup...." << std::endl;
	std::cout << "=======================================\n" << std::endl;

	/* Random seed to time */
	srand (time(NULL));

	std::cout << "Initializing SDL...\n" << std::endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to initialize SDL, see errorlog for details."<<std::endl;
		SDLerrorLogger("FATAL ERROR: SDL initialization");
		exit(EXIT_FAILURE);
	}
	std::cout << "SDL initialized!\n" << std::endl;
	std::cout << "=======================================\n" << std::endl;
#ifdef REBUILD_ASSETS
	std::cout << "Converting images...\n" << std::endl;
	if (!convert_all_images()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to convert images."<<std::endl;
		errorlogger("FATAL ERROR: Failed to convert images.");
		exit(EXIT_FAILURE);
	}
	std::cout << "Images converted!\n" << std::endl;
	std::cout << "=======================================\n" << std::endl;
	std::cout << "Converting models...\n" << std::endl;
	if (!convert_all_models()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to convert models."<<std::endl;
		errorlogger("FATAL ERROR: Failed to convert models.");
		exit(EXIT_FAILURE);
	}
	std::cout << "Models converted!\n" << std::endl;
	std::cout << "=======================================\n" << std::endl;
#endif
	/* ====================================== */

	/* MAIN VARS */
	/* ====================================== */

	std::cout << "Initializing main data structures...\n" << std::endl;

	std::cout << "------ Initializing resource_manager...\n" << std::endl;
	Resource_manager resource_manager;
	std::cout << "------ Resource_manager initialized!\n" << std::endl;

	std::cout << "------ Initializing renderer...\n" << std::endl;
	Renderer renderer(resource_manager);
	std::cout << "------ Renderer initialized!\n" << std::endl;

	std::cout << "------ Initializing state_handler...\n" << std::endl;
	State_handler state_handler(resource_manager);
	std::cout << "------ State_handler initialized!\n" << std::endl;

	std::cout << "------ Initializing world...\n" << std::endl;
	World world(resource_manager);
	std::cout << "------ World initialized!\n" << std::endl;

	std::cout << "------ Initializing timers...\n" << std::endl;
	Timer cap_timer;
	Timer move_timer;
	std::cout << "------ Timers initialized!\n" << std::endl;
	std::cout << "Sucessfully initialized main data structures!\n" << std::endl;
	std::cout << "=======================================\n" << std::endl;
	/* ====================================== */

	/* Main loop */
	/*=======================================================*/
	std::cout << "Engine running~\n" << std::endl;
	std::cout << "=======================================\n" << std::endl;

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
	/*Always be sure to clean up*/
	std::cout << "Engine shutting down...\n" << std::endl;
	SDL_Quit();
	std::cout << "Shutdown complete!\n" << std::endl;
	return 0;
}