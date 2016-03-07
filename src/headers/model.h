#ifndef MODEL_H
#define MODEL_H


/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
#include "renderer.h"
#include "mesh.h"
#include "resource_manager.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Resource_manager;
class Mesh;
class Renderer;

typedef std::shared_ptr<Mesh> Mesh_ptr;

class Model {
	private:
		std::vector<Mesh_ptr> meshes;
	public:
		Model();
		void render_model(const Renderer& renderer, const glm::vec3& position, const glm::vec3& size, const glm::vec3& direction)const;
		bool load_binary_model(const std::string& name, std::vector<std::string>& meshes);
		bool load_from_file(Resource_manager& manager, const std::string& name);
};

typedef std::shared_ptr<Model> Model_ptr;
/*=============================================*/

#endif

