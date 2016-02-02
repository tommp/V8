#ifndef MODEL_H
#define MODEL_H


/*Included headers*/
/*---------------------------------------------*/
#include "model_map.h"
#include "errorlogger.h"
#include "shader.h"
#include "mesh.h"
#include "resource_manager.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <unistd.h>
#include <vector>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Resource_manager;
class Mesh;
typedef std::shared_ptr<Mesh> Mesh_ptr;

class Model {
	private:
		std::vector<Mesh_ptr> meshes;
		Shader_ptr shader;
	public:
		Model();
		void render_model(const glm::vec3& position, const glm::vec3& size, GLfloat rotate)const;
		bool load_from_file(Resource_manager& manager, const std::string& name);
};

typedef std::shared_ptr<Model> Model_ptr;
/*=============================================*/

#endif

