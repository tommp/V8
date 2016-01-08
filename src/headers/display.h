#ifndef WINDOW_H
#define WINDOW_H


/*Included headers*/
/*---------------------------------------------*/
#include "./errorlogger.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <fstream>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Display{
private:
	SDL_Window* win;
    SDL_Renderer* ren;

    bool use_vsync;
    bool use_fullscreen;
    int width;
    int height;
    int screen_width;
    int screen_height;
public:
	Display();
	Display(const std::string& settings_file_path);
	int init_window();
	int init_renderer();
	int enable_fullscreen();
	int enable_vsync();
	int disable_vsync();

	void clear(){SDL_RenderClear(ren);};
	void present(){SDL_RenderPresent(ren);};

	SDL_Renderer* get_renderer(){return ren;};
	int get_screen_width(){return screen_width;};
	int get_screen_height(){return screen_height;};
	int get_width(){return width;};
	int get_height(){return height;};


};
/*=============================================*/

#endif