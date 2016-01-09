#ifndef WORLD_H
#define WORLD_H


/*Included headers*/
/*---------------------------------------------*/
#include "./actor.h"
#include "./player.h"
#include "./level.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class World {
private:
	Player player;
	Level_ptr current_level;
};
/*=============================================*/

#endif