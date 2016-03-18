#ifndef RENDERING_CONTEXT_H
#define RENDERING_CONTEXT_H

/*Included headers*/
/*---------------------------------------------*/
#include "enum_shader_type.h"
#include "enum_geom_type.h"
#include "glm.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <memory>
#include <list>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Shader;
class Material;

typedef std::shared_ptr<Shader> Shader_ptr;
typedef std::shared_ptr<Material> Material_ptr;

class Base_render_context{
public:
	GLuint VAO;
	GLuint num_vertices;
	Shader_type shader_type;
	GLenum render_mode;
	GLboolean render_elements;

	std::function<GLboolean(const Shader_ptr& shader)> set_base_uniforms;
	Material_ptr material;
	glm::vec4 object_color;
};

typedef std::shared_ptr<Base_render_context> Base_render_context_ptr;
typedef std::weak_ptr<Base_render_context> Base_render_context_weak;

class Rendering_context{
public:
	glm::mat4 model_matrix;
	GLboolean active;
	glm::vec3 init_direction;
	std::function<GLboolean(const Shader_ptr& shader)> set_top_uniforms;
	std::list<Base_render_context_weak> base_contexts;
};

typedef std::shared_ptr<Rendering_context> Rendering_context_ptr;
typedef std::weak_ptr<Rendering_context> Rendering_context_weak;
/*=============================================*/

#endif