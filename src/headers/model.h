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
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <string>
#include <unistd.h>
#include <list>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Resource_manager;
class Mesh;
class Renderer;
class Animation_set;

typedef std::shared_ptr<Animation_set> Animation_set_ptr;
typedef std::shared_ptr<Mesh> Mesh_ptr;

class Model{
	private:
		std::string name;
		std::string state;

		glm::mat4 model_matrix;
		glm::mat3 normal_model_matrix;
		glm::vec3 init_direction;

		std::list<Mesh_ptr> meshes;

		GLboolean is_animated;
		Animation_set_ptr animations;

		bool load_binary_model(Resource_manager& manager, 
							const std::string& name, 
							std::vector<std::string>& meshes);
		bool add_lambda_expression(const Mesh_ptr& mesh)const;
	public:
		Model();
		bool load_from_file(Resource_manager& manager, const std::string& name);
		bool update_matrices(const glm::vec3& position, 
							const glm::vec3& scale, 
							const glm::vec3& direction);
		bool add_mesh_contexts_to_renderer(Renderer& renderer)const;
		bool add_light_contexts_to_renderer(Renderer& renderer)const;
};

typedef std::shared_ptr<Model> Model_ptr;
/*=============================================*/

#endif

