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
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Material;
class Object;

typedef std::shared_ptr<Material> Material_ptr;

typedef struct Rendering_context {
	GLboolean active;

	GLuint VAO;
	GLuint num_vertices;
	glm::mat4 model_matrix;

	GLenum render_mode;

	Material_ptr material;
	glm::vec3 object_color;

	Shader_type shader_type;
}Rendering_context;

typedef std::shared_ptr<Rendering_context> Rendering_context_ptr;
typedef std::weak_ptr<Rendering_context> Rendering_context_weak;
/*=============================================*/

#endif