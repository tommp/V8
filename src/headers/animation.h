#ifndef ANIMATION_H
#define ANIMATION_H


/*Included headers*/
/*---------------------------------------------*/
#include "paths.h"
#include "animation_map.h"
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "frame.h"
#include "model.h"
#include "resource_manager.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
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

typedef struct Channel{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> scalings;
	std::vector<glm::fquat> rotations;
}Channel;

class Animation{
	private:
		GLdouble duration;
		GLdouble ticks_per_second;

		std::unordered_map<std::string, Channel> channels;

		GLuint num_frames;
		GLuint current_frame;
		GLuint current_frame_end;
		bool reverse_looping;
		bool going_forward;
	public:
		Animation();
		bool load_from_file(Resource_manager& resource_manager, const std::string& name);
		void render_frame(const Renderer& renderer, const glm::vec3& position, const glm::vec3& size, const glm::vec3& direction)const;
		void animate(const Renderer& renderer, const glm::vec3& position, const glm::vec3& size, const glm::vec3& direction);
		void reset_animation();
		void update_state();
		GLuint ret_frame(){return current_frame;};
};

typedef std::shared_ptr<Animation> Animation_ptr;
/*=============================================*/

#endif