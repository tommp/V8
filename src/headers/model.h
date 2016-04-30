#ifndef MODEL_H
#define MODEL_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "renderer.h"
#include "mesh.h"
#include "animation_set.h"
#include "rendering_context.h"
#include "resource_manager.h"
#include "base_light.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <string>
#include <unistd.h>
#include <list>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Resource_manager;
class Mesh;
class Renderer;
class Animation_set;
class Base_light;

typedef std::shared_ptr<Animation_set> Animation_set_ptr;
typedef std::shared_ptr<Mesh> Mesh_ptr;
typedef std::shared_ptr<Base_light> Light_ptr;

class Model{
	private:
		GLboolean is_complete;

		static GLuint context_count;

		std::string name;

		glm::vec3 init_direction;

		std::list<Mesh_ptr> meshes;

		std::list<Light_ptr> lights;

		GLboolean is_animated;
		Animation_set_ptr animations;

		bool load_binary_model(Resource_manager& manager, 
							const std::string& name, 
							std::vector<std::string>& meshes);
		bool add_mesh_contexts_to_renderer(Renderer& renderer)const;
		bool add_light_contexts_to_renderer(Renderer& renderer)const;
	public:
		Model();
		std::string get_name()const{return name;};
		bool load_from_file(Resource_manager& manager, const std::string& name);
		bool load_from_file(Resource_manager& manager, const std::string& name, const glm::vec4& color, GLfloat gloss);
		bool bind_context(const glm::mat4& model_matrix, std::string& context_name);
		bool unbind_context(std::string& context_name);
		bool add_contexts_to_renderer(Renderer& renderer)const;

	};

typedef std::shared_ptr<Model> Model_ptr;
/*=============================================*/

#endif

