#ifndef BUTTON_MAPPINGS_H
#define BUTTON_MAPPINGS_H


/*Included headers*/
/*---------------------------------------------*/
#include "./errorlogger.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Button_mappings{
	public:
		/* Player 1 control */
		SDL_Keycode up;
		SDL_Keycode down;
		SDL_Keycode left;
		SDL_Keycode right;
		SDL_Keycode jump;
		SDL_Keycode interact;
		SDL_Keycode menu;
		SDL_Keycode a0;
		SDL_Keycode a1;
		SDL_Keycode a2;
		SDL_Keycode a3;
		SDL_Keycode a4;

		/* Global commands */
		SDL_Keycode quit;
		Button_mappings();
};
/*=============================================*/

#endif