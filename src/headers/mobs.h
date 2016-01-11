#ifndef MOBS_H
#define MOBS_H

/*Included headers*/
/*---------------------------------------------*/
#include "./actor.h"
#include "./animation.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <string>
#include <unistd.h>
#include <unordered_map>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Slime_blob: public Character{
private:
	int speed;
	unsigned int last_move;
	unsigned int move_duration;
	int vec_x;
	int vec_y;
public:
	Slime_blob(SDL_Renderer& ren, Resource_manager& manager);
	void render_frame(SDL_Renderer& ren, SDL_Rect* offset);
	void update_position(float timedelta);
};
/*=============================================*/

#endif