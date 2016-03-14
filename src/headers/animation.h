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
#include <GL/glew.h>
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

		std::unordered_map<GLuint, Channel> channels;
	public:
		Animation();
		bool load_binary_animation(const std::string& name);
		bool load_from_file(const std::string& name);
};

typedef std::shared_ptr<Animation> Animation_ptr;
/*=============================================*/

#endif