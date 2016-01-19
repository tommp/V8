#include "./headers/display.h"

Display::Display(const std::string& settings_file_path){
	/* Init to standard settings as a fallback */
	use_vsync = true;
	use_fullscreen = true;
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
	screen_width = 0;
	screen_height = 0;

	/* Read from file here */
	/* Use settings external */
	/* =================== */

	/*Disables pesky screensavers while our wonderful graphics are beeing displayed*/
	SDL_DisableScreenSaver();

	init_window();
	init_renderer();
}

Display::Display(){
	/* Init to standard settings as a fallback */
	use_vsync = true;
	use_fullscreen = false;
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
	screen_width = 0;
	screen_height = 0;

	/*Disables pesky screensavers while our wonderful graphics are beeing displayed*/
	SDL_DisableScreenSaver();

	init_window();
	init_renderer();
}

int Display::init_window(){
	/*Initializes a window to render graphics in*/
	win = SDL_CreateWindow("Cradlands", 0, 0, width, height, 0);
	if (win == nullptr){
		SDLerrorLogger("SDL_CreateWindow");
		std::cout<<"Failed to create SDL window, see errorlog for details."<<std::endl;
		return 1;
	}

	if(use_fullscreen){
		enable_fullscreen();
	}

	/* MAKE SMOOTH WHEN MENU IN PLACE */
	SDL_ShowCursor(0);

	return 0;
}

int Display::init_renderer(){
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		SDLerrorLogger("SDL_CreateRenderer");
		std::cout<<"Failed to create SDL renderer, see errorlog for details."<<std::endl;
		return 1;
	}

	/* Select the color for drawing to black */
    SDL_SetRenderDrawColor(ren, 0, 255, 255, 255);

    /* Clear the entire screen to our selected color */
    SDL_RenderClear(ren);

	if(!use_vsync){
		disable_vsync();
	}

	if(use_fullscreen){
		int height_factor = screen_height / SCREEN_HEIGHT;
		int width_factor = screen_width / SCREEN_WIDTH;
		SDL_RenderSetScale(ren, height_factor, width_factor);
	}

	return 0;
}

int Display::enable_fullscreen(){
	SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_GetWindowSize(win, &screen_width, &screen_height);
	return 0;
}

int Display::enable_vsync(){
	SDL_GL_SetSwapInterval(1);
	return 0;
}

int Display::disable_vsync(){
	SDL_GL_SetSwapInterval(0);
	return 0;
}