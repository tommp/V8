#ifndef PROP_H
#define PROP_H

/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "model.h"
#include "object.h"
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

class Prop: public Actor, public Object{
private:
	Model_ptr model;
	std::string context_name;
	glm::mat4 model_matrix;
	
	glm::vec3 prev_position;
	glm::vec3 prev_scale;
	glm::vec3 prev_direction;

	bool update_matrices();
public:
	Prop();
	Prop(Resource_manager& manager, const std::string& model_name);
	Prop(Resource_manager& manager, 
		const std::string& model_name,
		const glm::vec3& position,
		const glm::vec3& scale,
		const glm::vec3& direction,
		GLfloat mass,
		Collision_shape shape);
	Prop(Resource_manager& manager, 
		const std::string& model_name,
		const glm::vec3& position,
		const glm::vec3& scale,
		const glm::vec3& direction,
		GLfloat mass,
		Collision_shape shape,
		const glm::vec3& collision_scale);
	~Prop();
	bool update_position(GLfloat timedelta);
	bool add_contexts_to_renderer(Renderer& renderer)const;
};

typedef std::shared_ptr<Prop> Prop_ptr;
/*=============================================*/

#endif