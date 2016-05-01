#ifndef ENTITY_H
#define ENTITY_H

/*Included headers*/
/*---------------------------------------------*/
#include "actor.h"
#include "model_context.h"
#include "object.h"
#include "base_controller.h"
#include "enum_collision_shape_type.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <GL/glew.h>
#include <unistd.h>
#include <iostream>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

class Entity: public Actor{
	private:
		GLboolean billboarded;

		std::string name;
		Resource_manager* manager;

		GLboolean has_model;
		Model_context model_context;

		GLboolean has_physics;
		Object physics_context;
	public:
		Entity();
		Entity(const std::string& filename);
		Entity(Resource_manager* manager): manager(manager){};

		bool save_entity();
		bool load_entity(const std::string& filename);

		bool init_entity(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& dir);

		bool init_model_context(const std::string& model_name);
		bool init_model_context(const std::string& model_name, 
								const glm::vec4& color, 
								GLfloat gloss);

		bool init_physics_context(Collision_shape shape, 
								GLfloat mass, 
								GLboolean allow_deactivation);

		bool update_position();
		bool update_physics_context();
		bool update_model_context(const glm::mat4& view_matrix);

		bool synchronize_contexts(const glm::mat4& view_matrix);

		/* Script funcs */
		bool clean_rot_n_scale(glm::mat4& matrix);

};


typedef std::shared_ptr<Entity> Entity_ptr;
/*=============================================*/

#endif