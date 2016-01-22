#ifndef MOBS_H
#define MOBS_H

/*Included headers*/
/*---------------------------------------------*/
#include "character.h"
#include "animation.h"
#include "glm.h"
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
class Cube: public Character{
private:
	glm::vec3 last_pos;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	unsigned int speed;
	unsigned int last_move;
	unsigned int move_duration;
public:
	Cube(Resource_manager& manager);
	void render_frame();
	void update_position(float timedelta);
	void touch_character(Character& character);
};
/*=============================================*/

#endif