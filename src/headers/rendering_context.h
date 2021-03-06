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
		GLenum primitive_type;
		GLboolean render_elements;

		std::function<GLboolean(const Shader_ptr& shader)> setup_base_uniforms;
		std::unordered_map<std::string, std::function<GLboolean(const Shader_ptr& shader, const glm::mat4& view_matrix, GLuint instance, GLboolean only_model)>> instance_uniform_setups;
};

typedef std::shared_ptr<Rendering_context> Rendering_context_ptr;
typedef std::weak_ptr<Rendering_context> Rendering_context_weak;

class Rendering_context_light{
	public:
		Shader_type shader_type;

		std::function<GLboolean(GLuint buffer, const glm::mat4& view, GLuint instance)> setup_base_uniforms;
};

typedef std::shared_ptr<Rendering_context_light> Rendering_context_light_ptr;
typedef std::weak_ptr<Rendering_context_light> Rendering_context_light_weak;
/*=============================================*/

#endif