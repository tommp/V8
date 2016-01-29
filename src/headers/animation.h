#ifndef ANIMATION_H
#define ANIMATION_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "texture.h"
#include "frame.h"
#include "resource_manager.h"
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
#include <vector>
#include <cstdlib>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
#define ANIMATION_DATA_FILE_PATH "../data/content.boogie"

class Resource_manager;

class Animation{
	private:
		std::vector<Frame> frames;

		GLuint num_frames;
		GLuint current_frame;
		GLuint current_frame_end;
		bool reverse_looping;
		bool going_forward;
	public:
		Animation();
		bool load_from_file(Resource_manager& resource_manager, const std::string& name);
		void render_current(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, GLfloat rotate);
		void reset_animation();
};

typedef std::shared_ptr<Animation> Animation_ptr;
/*=============================================*/

#endif