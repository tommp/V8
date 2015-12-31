#include "./headers/main.h"

int main(int argc, char** argv){

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

	/*Initializes the renderer to draw in*/
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		SDLerrorLogger("SDL_CreateRenderer");
		std::cout<<"Failed to create SDL renderer, see errorlog for details."<<std::endl;
		return 1;
	}

	// Select the color for drawing. It is set to red here.
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

    // Clear the entire screen to our selected color.
    SDL_RenderClear(ren);

    srand (time(NULL));

    Texture bloom;
    bloom.loadFromFile(ren, "../pixelart/Foilage/SharpBush.png");
    for (int i = 0; i < 3000; i++){
    	bloom.render(ren, (rand() % WIDTH) - (bloom.getWidth() / 2), (rand() % HEIGHT) - (bloom.getHeight() / 2) );
    }

	SDL_RenderPresent(ren);
	
	/*
    bool gametime = true;
    while(gametime){

    }*/

    /*Waits for user input before quitting*/
    waitForEvent();

    /*Always be sure to clean up*/
    SDL_Quit();
    return 0;
}