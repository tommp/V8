#ifndef PLAYER_H
#define PLAYER_H


/*Included headers*/
/*---------------------------------------------*/
#include "enum_key_type.h"
#include "object.h"
#include "model.h"
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
class Model;
class Resource_manager;
class Object;
class btRigidBody;

class Player : public Object{
private:
	Model_ptr model;
	std::string state;
	glm::vec3 velocity;
	glm::vec3 init_direction;
	GLfloat speed;
	Resource_manager* manager;
public:
	Player(Resource_manager& init_manager);
	bool update_position(GLfloat timedelta);
	bool update_context();
	bool touch_object(Object& Object);
	bool add_context_to_renderer(Renderer& renderer)const;
};

typedef std::shared_ptr<Player> Player_ptr;
/*=============================================*/

#endif