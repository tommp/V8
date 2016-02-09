#ifndef MOBS_H
#define MOBS_H

/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "character.h"
#include "animation.h"
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
class Animation;
class Character;

class Cube: public Character{
private:
	glm::vec3 last_pos;
	glm::vec3 velocity;

	unsigned int speed;
	unsigned int last_move;
	unsigned int move_duration;
public:
	Cube(Resource_manager& manager);
	~Cube();
	void render_frame(const Renderer& renderer)const;
	void update_position(float timedelta);
	void touch_character(Character& character);
};

typedef std::shared_ptr<Cube> Cube_ptr;
/*=============================================*/

#endif