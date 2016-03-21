#ifndef PLAYER_H
#define PLAYER_H


/*Included headers*/
/*---------------------------------------------*/
#include "enum_key_type.h"
#include "model.h"
#include "resource_manager.h"
#include "object.h"
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
class Model;
class Resource_manager;
class btRigidBody;

class Player: public Object{
private:
	Model_ptr model;
	glm::vec3 velocity;
	glm::vec3 init_direction;
	GLfloat speed;
	Resource_manager* manager;

	/* Used for optimization */
	glm::vec3 prev_position;
	glm::vec3 prev_size;
	glm::vec3 prev_direction;
public:
	Player(Resource_manager& init_manager);
	bool update_position(GLfloat timedelta);
	bool update_context();
	void update_model_matrix();
	bool touch_object(Object& object);
};

typedef std::shared_ptr<Player> Player_ptr;
/*=============================================*/

#endif