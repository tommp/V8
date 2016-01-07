#include "./headers/main.h"

int main(int argc, char** argv){

	/* MAIN VARS */
	/* ====================================== */

	Resource_manager resource_manager;

	/* Main button map */
	Button_mappings button_mappings;

	/* Main state handler */
	State_handler state_handler(&button_mappings);

    /* Random seed to time */
    srand (time(NULL));

    Timer cap_timer;
    SDL_Window *win;
    SDL_Renderer *ren;
    /* ====================================== */

	/*Initializes SDL for graphical display*/
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		SDLerrorLogger("SDL initialization");
		std::cout<<"Failed to initialize SDL, see errorlog for details."<<std::endl;
		return 1;
	}

	/*Disables pesky screensavers while our wonderful graphics are beeing displayed*/
	SDL_DisableScreenSaver();

	/*Initializes a window to render graphics in*/
	win = SDL_CreateWindow("Cradlands", 0, 0, WIDTH, HEIGHT, 0);
	if (win == nullptr){
		SDLerrorLogger("SDL_CreateWindow");
		std::cout<<"Failed to create SDL window, see errorlog for details."<<std::endl;
		return 1;
	}

	/* CLEAN THIS UP */
	int ww, wh;
	SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_GetWindowSize(win, &ww, &wh);

	/*Initializes the renderer to draw in*/
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		SDLerrorLogger("SDL_CreateRenderer");
		std::cout<<"Failed to create SDL renderer, see errorlog for details."<<std::endl;
		return 1;
	}

	/* Select the color for drawing to black */
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

    /* Clear the entire screen to our selected color */
    SDL_RenderClear(ren);


    /* TESTCODE */
    /*=======================================================*/
    resource_manager.load_texture(ren, "../pixelart/Foilage/SharpBush.png", "bush");
    Texture bloom = *resource_manager.get_texture_ptr("bush");
    /* ===================================================== */

	/* Main loop */
	/*=======================================================*/
    while(state_handler.game_is_running()){

    	//Start cap timer
        cap_timer.start();

	    /* Handle events in the queue */
	    state_handler.handle_events();

	    /* Clear the screen */
	    SDL_RenderClear(ren);

	    for (int i = 0; i < 10000; i++){
	    	bloom.render(ren, (rand() % ww) - (bloom.get_width() / 2), (rand() % wh) - (bloom.get_height() / 2) );
	    }

	    /* Render and wait */
	    SDL_RenderPresent(ren);

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