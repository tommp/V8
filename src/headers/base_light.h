#ifndef BASE_LIGHT_H
#define BASE_LIGHT_H

/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "enum_light_type.h"
#include "errorlogger.h"
#include "utility.h"
#include "rendering_context.h"
#include "actor.h"
#include "renderer.h"
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

typedef std::shared_ptr<Shader> Shader_ptr;

class Base_light: public Actor{
	protected:
		glm::vec3 color;

		/* x is ambient, y is diffuse, z is specular */
		glm::vec3 color_components;

		GLboolean render_shadows;
		GLboolean apply_SSAO;

		Rendering_context_light_ptr base_light_context;
		glm::mat4 quad_model_matrix;
		
	public:
		Base_light();
		Shader_type get_type(){return base_light_context->shader_type;};
		bool randomize_position(const glm::i16vec3& maxpos, const glm::i16vec3& offset);
		bool randomize_color(GLfloat intensity);
		bool randomize_diffuse();
		bool randomize_ambient();
		bool randomize_specular();
		bool add_context(Renderer& renderer)const;
		bool set_direction(const glm::vec3& dir);
		bool set_position(const glm::vec3& pos);
};

typedef std::shared_ptr<Base_light> Light_ptr;
/*=============================================*/

#endif