#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H


/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
#include "resource_manager.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class State_handler{
private:
	/* Main event handler */
	SDL_Event ehandler;

	const Resource_manager* manager;

	bool game_running;
	bool menu_open;
	bool paused;
public:
	State_handler(const Resource_manager& init_manager);
	void handle_events();
	bool game_is_running(){return game_running;};
};

/*=============================================*/

#endif