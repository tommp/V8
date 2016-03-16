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

class Model {
	private:
		std::string name;
		std::list<Mesh_ptr> meshes;
		
		GLboolean is_animated;
		Animation_set_ptr animations;
	public:
		Model();
		bool add_bases_to_context(Rendering_context& context)const;
		bool load_binary_model(Resource_manager& manager, const std::string& name, std::vector<std::string>& meshes);
		bool load_from_file(Resource_manager& manager, const std::string& name);
};

typedef std::shared_ptr<Model> Model_ptr;
/*=============================================*/

#endif

