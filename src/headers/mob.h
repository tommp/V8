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
	glm::mat4 model_matrix;
	glm::mat3 normal_model_matrix;
	std::string context_name;

	glm::vec3 velocity;
	glm::vec3 last_pos;

	GLuint speed;
	GLuint last_move;
	GLuint move_duration;

	/* Used for optimization */
	glm::vec3 prev_position;
	glm::vec3 prev_scale;
	glm::vec3 prev_direction;
	bool update_matrices();

public:
	Mob();
	Mob(Resource_manager& manager, const std::string& model_name, const std::string& mob_name);
	Mob(Resource_manager& manager, 
		const std::string& model_name, 
		const std::string& mob_name,
		const glm::vec3& position,
		const glm::vec3& scale,
		const glm::vec3& direction,
		GLfloat mass,
		Collision_shape shape);
	~Mob();
	bool update_position(GLfloat timedelta);
	bool add_contexts_to_renderer(Renderer& renderer)const;
};

typedef std::shared_ptr<Mob> Mob_ptr;
/*=============================================*/

#endif