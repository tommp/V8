#ifndef PLAYER_H
#define PLAYER_H


/*Included headers*/
/*---------------------------------------------*/
#include "keys.h"
#include "character.h"
#include "animation.h"
#include "resource_manager.h"
#include "world.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Player : public Character{
private:
	glm::vec3 velocity;
	GLfloat speed;
	Resource_manager* manager;
public:
	Player(Resource_manager& init_manager);
	void render_frame()const;
	void update_position(GLfloat timedelta);
	void touch_character(Character& character);
};
/*=============================================*/

#endif