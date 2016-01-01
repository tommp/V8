#include "./headers/main.h"

int main(int argc, char** argv){

	/* MAIN VARS */
	/* ====================================== */
	/* Main event handler */
	SDL_Event ehandler;

	/* Main loop variable */
    bool gametime = true;

    /* Random seed to time */
    srand (time(NULL));

    Timer cap_timer;
    Timer fps_timer;
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
	SDL_Window *win = SDL_CreateWindow("Cradlands", 0, 0, WIDTH, HEIGHT, 0);
	if (win == nullptr){
		SDLerrorLogger("SDL_CreateWindow");
		std::cout<<"Failed to create SDL window, see errorlog for details."<<std::endl;
		return 1;
	}
	int ww, wh;
	SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_GetWindowSize(win,
                       &ww,
                       &wh);

	/*Initializes the renderer to draw in*/
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
    Texture bloom;
    bloom.load_from_file(ren, "../pixelart/Foilage/SharpBush.png");
    /* ===================================================== */


	/* Main loop */
	/*=======================================================*/
    while(gametime){

    	//Start cap timer
        cap_timer.start();

	    /* Handle events in the queue */
	    while( SDL_PollEvent( &ehandler ) != 0 )
	    {
	        /* User requests quit */
	        if( ehandler.type == SDL_QUIT ){
	            gametime = false;
	        }
	        else if( ehandler.type == SDL_KEYDOWN ){
	        	if (ehandler.key.keysym.sym == SDLK_ESCAPE) {
	        		gametime = false;
	        	}
	        }
	    }
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