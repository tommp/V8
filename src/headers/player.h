#ifndef PLAYER_H
#define PLAYER_H


/*Included headers*/
/*---------------------------------------------*/
#include "keys.h"
#include "character.h"
#include "animation.h"
#include "resource_manager.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <unistd.h>
#include <iostream>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Animation;
class Resource_manager;
class Character;
class btRigidBody;

class Player : public Character{
private:
	glm::vec3 velocity;
	glm::vec3 init_direction;
	GLfloat speed;
	Resource_manager* manager;
public:
	Player(Resource_manager& init_manager);
	void render_frame(const Renderer& renderer)const;
	bool update_position(GLfloat timedelta);
	bool touch_character(Character& character);
};

typedef std::shared_ptr<Player> Player_ptr;
/*=============================================*/

#endif