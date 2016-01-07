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
private:
	char up;
	char down;
	char left;
	char right;
	char jump;
	char interact;
	char menu;
	char a0;
	char a1;
	char a2;
	char a3;
	char a4;
	char a5;
	char a6;
	char a7;
	char a8;
	char a9;
	char a10;
	char a11;
	char a12;
	char a13;
	char a14;
	char a15;
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