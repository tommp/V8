#ifndef MOBS_H
#define MOBS_H

/*Included headers*/
/*---------------------------------------------*/
#include "./character.h"
#include "./animation.h"
#include "./vector.h"
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
	Vec3 velocity;
	unsigned int speed;
	unsigned int last_move;
	unsigned int move_duration;
public:
	Slime_blob(Resource_manager& manager);
	void render_frame();
	void update_position(float timedelta);
	void touch_character(Character& character);
};
/*=============================================*/

#endif