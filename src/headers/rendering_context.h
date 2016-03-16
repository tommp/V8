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
class Material;
class Object;

typedef std::shared_ptr<Material> Material_ptr;

typedef struct Base_render_context{
	GLuint VAO;
	GLuint num_vertices;
	
	GLenum render_mode;

	Material_ptr material;
	glm::vec4 object_color;

	Shader_type shader_type;
}Base_render_context;

typedef std::shared_ptr<Base_render_context> Base_render_context_ptr;
typedef std::weak_ptr<Base_render_context> Base_render_context_weak;

typedef struct Rendering_context{
	glm::mat4 model_matrix;
	GLboolean active;
	glm::vec3 init_direction;
	std::list<Base_render_context_weak> base_contexts;
}Rendering_context;

typedef std::shared_ptr<Rendering_context> Rendering_context_ptr;
typedef std::weak_ptr<Rendering_context> Rendering_context_weak;
/*=============================================*/

#endif