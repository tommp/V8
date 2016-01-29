#ifndef ANIMATION_SET_H
#define ANIMATION_SET_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "resource_manager.h"
#include "texture.h"
#include "animation.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
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
	public:
		Animation_set();
		void render_current(const std::string& name, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, GLfloat rotate);
		bool load_from_file(Resource_manager& resource_manager, const std::string& name);
};

typedef std::shared_ptr<Animation_set> Animation_set_ptr;
/*=============================================*/

#endif