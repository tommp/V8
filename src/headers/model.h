#ifndef MODEL_H
#define MODEL_H


/*Included headers*/
/*---------------------------------------------*/
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

class Model: public Actor{
	private:
		std::string name;
		std::string state;

		glm::mat4 model_matrix;
		glm::mat4 normal_model_matrix;
		glm::vec3 init_direction;

		std::list<Mesh_ptr> meshes;
		std::unordered_map<std::string, Rendering_context_weak> mesh_contexts;

		GLboolean is_animated;
		Animation_set_ptr animations;

		bool load_binary_model(Resource_manager& manager, 
							const std::string& name, 
							std::vector<std::string>& meshes);
	public:
		Model();
		std::unordered_map<std::string, Rendering_context_weak>* get_contexts()const;
		
		bool load_from_file(Resource_manager& manager, const std::string& name);
		bool add_lambda_expression(const Mesh_ptr& mesh)const;
		virtual bool update_position(float timedelta);
		virtual bool touch_object();
		btRigidBody* get_collision_body()const;
		bool update_matrices();
};

typedef std::shared_ptr<Model> Model_ptr;
/*=============================================*/

#endif

