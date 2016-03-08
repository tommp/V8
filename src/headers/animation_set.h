#ifndef ANIMATION_SET_H
#define ANIMATION_SET_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "skeletal_node.h"
#include "utility.h"
#include "resource_manager.h"
#include "animation.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <string>
#include <unistd.h>
#include <list>
#include <fstream>
#include <sstream>
#include <memory>
#include <unordered_map>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Resource_manager;
class Animation;

typedef std::shared_ptr<Animation> Animation_ptr;

class Animation_set {
	private:
		Mesh_ptr mesh;
		std::unordered_map<std::string, Animation_ptr> animations;
		Skeletal_node_ptr root_node;
	public:
		Animation_set();
		void render_current(const Renderer& renderer, const std::string& name, const glm::vec3& position, const glm::vec3& size, const glm::vec3& direction)const;
		bool load_from_file(Resource_manager& resource_manager, const std::string& name);
};

typedef std::shared_ptr<Animation_set> Animation_set_ptr;
/*=============================================*/

#endif