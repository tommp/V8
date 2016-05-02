#ifndef ENTITY_H
#define ENTITY_H

/*Included headers*/
/*---------------------------------------------*/
#include "actor.h"
#include "model_context.h"
#include "object.h"
#include "base_controller.h"
#include "enum_collision_shape_type.h"
#include "enum_key_type.h"
#include "base_light.h"
#include "dir_light.h"
#include "point_light.h"
#include "spot_light.h"
#include "physics_engine.h"
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
class Physics_engine;

class Entity: public Actor{
	private:
		std::string name;
		std::string loop_script_name;
		std::string init_script_name;

		std::vector<std::vector<std::vector<GLuint>>> script_matrix;

		Resource_manager* manager;

		/* User buffers */
		std::vector<GLboolean> bool_buffer;
		std::vector<GLint> int_buffer;
		std::vector<GLfloat> float_buffer;
		std::vector<glm::vec3> vec3_buffer;

		/* Script variable call buffers */
		std::vector<GLboolean> bool_vars;
		std::vector<GLint> int_vars;
		std::vector<GLfloat> float_vars;
		std::vector<glm::vec3> vec3_vars;
		std::vector<std::string> string_vars;


		GLboolean has_model;
		GLboolean billboarded;
		Model_context model_context;

		GLboolean has_physics;
		Object physics_context;

		std::forward_list<Light_ptr> lights;

		bool update_physics_context();
		bool update_model_context();
	public:
		Entity();
		Entity(const std::string& filename);
		Entity(Resource_manager* manager): manager(manager){};

		virtual bool update(GLfloat timedelta);

		bool save_entity()const;
		bool load_entity(const std::string& filename);

		void init(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& dir);
		bool init_model_context(const std::string& model_name);
		bool init_model_context(const std::string& model_name, 
								const glm::vec4& color, 
								GLfloat gloss);

		bool init_physics_context(Collision_shape shape, 
								GLfloat mass, 
								GLboolean allow_deactivation);

		bool update_contexts();

		bool add_contexts_to_renderer(Renderer& renderer)const;
		bool add_model_contexts_to_renderer(Renderer& renderer)const;
		bool add_light_contexts_to_renderer(Renderer& renderer)const;

		bool add_physics_context_to_physics_engine(Physics_engine& physics_engine);

		btRigidBody* get_collision_body()const;

		/* Script funcs */
		bool clean_rot_n_scale(glm::mat4& matrix)const;
		bool check_if_mapped_button_pressed(const std::string& map, const Key& key)const;
		bool set_linear_velocity(const glm::vec3& velocity);
		bool inc_linear_velocity(const glm::vec3& velocity);
		bool set_direction(const glm::vec3& direction);

		bool resize_vec3_buffer(GLuint buffer_size);
		bool set_vec3_buffer(GLuint index, const glm::vec3& data);
		bool inc_vec3_buffer(GLuint index, const glm::vec3& data);
		bool normalize_vec3_in_buffer(GLuint index);

		bool resize_int_buffer(GLuint buffer_size);
		bool set_int_buffer(GLuint index, GLint data);
		bool inc_int_buffer(GLuint index, GLint data);

		bool resize_float_buffer(GLuint buffer_size);
		bool set_float_buffer(GLuint index, GLfloat data);
		bool inc_float_buffer(GLuint index, GLfloat data);

		bool resize_bool_buffer(GLuint buffer_size);
		bool set_bool_buffer(GLuint index, GLboolean data);
		bool toggle_bool_buffer(GLuint index);

		bool int_compare(GLint a, GLint b);
		bool float_compare(GLfloat a, GLfloat b);
};

typedef std::shared_ptr<Entity> Entity_ptr;
/*=============================================*/

#endif