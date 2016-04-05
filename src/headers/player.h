#ifndef PLAYER_H
#define PLAYER_H


/*Included headers*/
/*---------------------------------------------*/
#include "enum_key_type.h"
#include "model.h"
#include "resource_manager.h"
#include "object.h"
#include "spot_light.h"
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

class Player: public Actor, public Object{
private:
	Model_ptr model;
	glm::mat4 model_matrix;
	glm::mat3 normal_model_matrix;
	std::string context_name;
	Spot_light_ptr flashlight;

	glm::vec3 velocity;

	GLfloat speed;
	
	Resource_manager* manager;

	/* Used for optimization */
	glm::vec3 prev_position;
	glm::vec3 prev_scale;
	glm::vec3 prev_direction;

	bool update_matrices(const glm::mat4& view_matrix);
public:
	~Player();
	Player();
	Player(Resource_manager& init_manager, const std::string& model_name);
	bool update_position(GLfloat timedelta, const glm::mat4& view_matrix);
	bool touch_object(Object& object);
	bool add_contexts_to_renderer(Renderer& renderer)const;
};

typedef std::shared_ptr<Player> Player_ptr;
/*=============================================*/

#endif