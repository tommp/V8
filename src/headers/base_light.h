#ifndef BASE_LIGHT_H
#define BASE_LIGHT_H

/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "enum_light_type.h"
#include "errorlogger.h"
#include "utility.h"
#include "rendering_context.h"
#include "object.h"
#include "actor.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <GL/glew.h>
#include <string>
#include <unistd.h>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Shader;
class Model;

typedef std::shared_ptr<Model> Model_ptr;
typedef std::shared_ptr<Shader> Shader_ptr;

class Base_light: public Object, public Actor{
	protected:
		/* Light rendering specifics */
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		Rendering_context_ptr base_light_context;
		GLuint quad_VBO;
		GLuint quad_EBO;
		glm::mat4 quad_model_matrix;
		glm::vec3 light_volume_scale;
		
		/* Object rendering specifics */
		Model_ptr model;
		
		bool init_light_volume();
	public:
		Base_light();
		virtual bool render_light(const Shader_ptr& shader)const = 0;
		Shader_type get_type(){return base_light_context->shader_type;};
		bool update_position(GLfloat timedelta);
		bool update_context();
		bool touch_object(Object& object);
		bool add_bases_to_context();
		bool randomize_position(const glm::i16vec3& maxpos, const glm::i16vec3& offset);	
		bool randomize_diffuse(const glm::i16vec3& max);
		bool randomize_ambient(const glm::i16vec3& max);
		bool randomize_specular(const glm::i16vec3& max);
		bool free_buffers();
};
typedef std::shared_ptr<Base_light> Light_ptr;
/*=============================================*/

#endif