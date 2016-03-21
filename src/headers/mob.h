#ifndef MOBS_H
#define MOBS_H

/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "model.h"
#include "object.h"
#include "actor.h"
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
class btRigidBody;

class Mob: public Actor, public Object{
private:
	std::string name;

	Model_ptr model;

	glm::vec3 velocity;
	glm::vec3 last_pos;

	GLuint speed;
	GLuint last_move;
	GLuint move_duration;

	/* Used for optimization */
	glm::vec3 prev_position;
	glm::vec3 prev_size;
	glm::vec3 prev_direction;
public:
	Mob(Resource_manager& manager, const std::string& model_name, const std::string& mob_name);
	~Mob();
	bool update_position(GLfloat timedelta);
	bool touch_object(Object& object);
	bool add_contexts_to_renderer(Renderer& renderer)const;
};

typedef std::shared_ptr<Mob> Mob_ptr;
/*=============================================*/

#endif