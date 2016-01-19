#ifndef EVENTS_H
#define EVENTS_H


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

class State_handler;
class Button_mappings;

class Button_mappings{
	friend class State_handler;
	friend class Player;
private:
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
public:
	Button_mappings();
};


class State_handler{
private:
	/* Main event handler */
	SDL_Event ehandler;

	Button_mappings* mappings;

	bool game_running;
	bool menu_open;
	bool paused;
public:
	State_handler(Button_mappings* map);
	void handle_events();
	bool game_is_running(){return game_running;};
};

/*=============================================*/

#endif