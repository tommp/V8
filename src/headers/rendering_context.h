#ifndef RENDERING_CONTEXT_H
#define RENDERING_CONTEXT_H

/*Included headers*/
/*---------------------------------------------*/
#include "enum_shader_type.h"
#include "glm.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <memory>
#include <unordered_map>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Shader;
class Material;

typedef std::shared_ptr<Shader> Shader_ptr;
typedef std::shared_ptr<Material> Material_ptr;

class Rendering_context{
	public:
		GLuint VAO;
		GLuint num_vertices;
		Shader_type shader_type;
		GLenum render_mode;
		GLboolean render_elements;

		std::function<GLboolean(const Shader_ptr& shader)> setup_base_uniforms;
		std::unordered_map<std::string, std::function<GLboolean(const Shader_ptr& shader)>> instance_uniform_setups;
};

typedef std::shared_ptr<Rendering_context> Rendering_context_ptr;
typedef std::weak_ptr<Rendering_context> Rendering_context_weak;
/*=============================================*/

#endif