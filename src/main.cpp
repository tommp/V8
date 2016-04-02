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

	std::cout << "Initializing SDL..." << std::endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to initialize SDL, see errorlog for details."<<std::endl;
		SDLerrorLogger("FATAL ERROR: SDL initialization");
		exit(EXIT_FAILURE);
	}
	std::cout << "SDL initialized!\n" << std::endl;
	std::cout << "=======================================\n" << std::endl;

	std::cout << "Initializing utility variables..." << std::endl;
	if (!init_utility_vars()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to initialize utility variables!"<<std::endl;
		errorlogger("FATAL ERROR: Failed to initialize utility variables!");
		exit(EXIT_FAILURE);
	}
	std::cout << "Utility variables initialized!\n" << std::endl;
	std::cout << "=======================================\n" << std::endl;

#ifdef REBUILD_ASSETS
	std::cout << "Converting images..." << std::endl;
	if (!convert_all_images()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to convert images."<<std::endl;
		errorlogger("FATAL ERROR: Failed to convert images.");
		exit(EXIT_FAILURE);
	}
	std::cout << "Images converted!\n" << std::endl;
	std::cout << "=======================================\n" << std::endl;
	std::cout << "Converting models..." << std::endl;
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

	std::cout << "------ Initializing resource_manager..." << std::endl;
	Resource_manager resource_manager;
	std::cout << "------ Resource_manager initialized!\n" << std::endl;

	std::cout << "------ Initializing renderer..." << std::endl;
	Renderer renderer(resource_manager);
	std::cout << "------ Renderer initialized!\n" << std::endl;

	std::cout << "------ Initializing state_handler..." << std::endl;
	State_handler state_handler(resource_manager, renderer);
	std::cout << "------ State_handler initialized!\n" << std::endl;

	std::cout << "------ Initializing world..." << std::endl;
	World world(resource_manager, renderer);
	std::cout << "------ World initialized!\n" << std::endl;

	std::cout << "------ Initializing timers..." << std::endl;
	Timer cap_timer;
	Timer move_timer;
	Timer performance_timer;
	std::cout << "------ Timers initialized!\n" << std::endl;
	std::cout << "Sucessfully initialized main data structures!\n" << std::endl;
	std::cout << "=======================================\n" << std::endl;
	/* ====================================== */

	/* Main loop */
	/*=======================================================*/
	std::cout << "Engine running~\n" << std::endl;
	std::cout << "=======================================" << std::endl;
	std::cout << "=======================================\n" << std::endl;

	GLuint average_render_time = 0;
	GLuint pooled_render_time = 0;
	GLuint num_frames_average = 30;
	GLuint frame_counter = 0;
	GLuint peak_ms = 0;
	while(state_handler.game_is_running()){
		performance_timer.restart();
#if DISABLE_VSYNC
		//Start cap timer
		cap_timer.restart();
#endif

		/* Handle events in the queue */
		state_handler.handle_events();

		/* Update the position of all world objects */
		float timedelta = move_timer.get_ticks() / 1000.0f;
		if (!world.update_positions(timedelta, renderer)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to update world positions!" << std::endl;
			errorlogger("FATAL ERROR: Failed to update world positions!");
			exit(EXIT_FAILURE);
		}
		move_timer.restart();

		/* Resolve all collisions */
		if (!world.resolve_collisions()){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to resolve world collisions!" << std::endl;
			errorlogger("FATAL ERROR: Failed to resolve world collisions!");
			exit(EXIT_FAILURE);
		}

		/* Render to screen */
		
		if(!world.render_world(renderer)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to render world!" << std::endl;
			errorlogger("FATAL ERROR: Failed to render world!");
			exit(EXIT_FAILURE);
		}

#if DISABLE_VSYNC
		/* If frame finished early */
		int frame_ticks = cap_timer.get_ticks();
		if (frame_ticks < Utility_consts::SCREEN_TICKS_PER_FRAME) {
			/* Wait remaining time */
			SDL_Delay(Utility_consts::SCREEN_TICKS_PER_FRAME - frame_ticks);
		}
#endif
		GLint performance_ticks = performance_timer.get_ticks();
		performance_timer.pause();
		if (frame_counter < num_frames_average) {
			++frame_counter;
			pooled_render_time += performance_ticks;
		}
		else{
			frame_counter = 0;
			average_render_time = pooled_render_time / num_frames_average;
			if (average_render_time > peak_ms) {
				peak_ms = average_render_time;
			}
			pooled_render_time = 0;
		}

		std::cout << '\r' << std::setw(5) << std::setfill('0') << "Avg. render time: "<< average_render_time << " ms" << "      Peak render time: " << peak_ms << "ms" << std::flush;
	}
	/*=======================================================*/
	std::cout << "\n=======================================" << std::endl;
	std::cout << "=======================================\n" << std::endl;
	std::cout << "Engine shutting down...\n" << std::endl;
	SDL_Quit();
	std::cout << "Shutdown complete!\n" << std::endl;
	return 0;
}