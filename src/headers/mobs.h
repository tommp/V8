#ifndef MOBS_H
#define MOBS_H

/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "object.h"
#include "model.h"
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
class Model;
class Object;
class btRigidBody;

class Cube: public Object{
private:
	Model_ptr model;
	glm::vec3 last_pos;
	glm::vec3 velocity;

	unsigned int speed;
	unsigned int last_move;
	unsigned int move_duration;

	/* Used for optimization */
	glm::vec3 prev_position;
	glm::vec3 prev_size;
	glm::vec3 prev_direction;
public:
	Cube(Resource_manager& manager);
	~Cube();
	bool update_position(float timedelta);
	bool update_context();
	void update_model_matrix();
	bool touch_object(Object& Object);
	bool add_context_to_renderer(Renderer& renderer)const;
	bool add_bases_to_context();
};

typedef std::shared_ptr<Cube> Cube_ptr;
/*=============================================*/

#endif