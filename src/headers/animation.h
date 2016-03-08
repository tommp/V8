#ifndef ANIMATION_H
#define ANIMATION_H


/*Included headers*/
/*---------------------------------------------*/
#include "paths.h"
#include "channel.h"
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "model.h"
#include "resource_manager.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <unistd.h>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <memory>
#include <unordered_map>
#include <vector>
#include <cstdlib>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

class Resource_manager;
class Frame;
class Model;

class Animation{
	private:
		GLdouble duration;
		GLdouble ticks_per_second;

		std::unordered_map<std::string, Channel> channels;

		bool reverse_looping;
		bool going_forward;
	public:
		Animation();
		void render_frame(const Renderer& renderer, const glm::vec3& position, const glm::vec3& size, const glm::vec3& direction)const;
		void animate(const Renderer& renderer, const glm::vec3& position, const glm::vec3& size, const glm::vec3& direction);
		void update_state();
};

typedef std::shared_ptr<Animation> Animation_ptr;
/*=============================================*/

#endif