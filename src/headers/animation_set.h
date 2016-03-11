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
		std::unordered_map<std::string, Animation_ptr> animations;
		Skeletal_node_ptr root_node;
	public:
		Animation_set();
		bool load_from_file(Resource_manager& resource_manager, const std::string& name);
		bool load_binary_animation_set(Resource_manager& resource_manager, const std::string& name);
		bool load_binary_skeleton(std::ifstream& contentf, const std::string& name);
};

typedef std::shared_ptr<Animation_set> Animation_set_ptr;
/*=============================================*/

#endif