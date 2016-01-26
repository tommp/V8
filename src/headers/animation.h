#ifndef ANIMATION_H
#define ANIMATION_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "mesh.h"
#include "utility.h"
#include "texture.h"
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
		Mesh_ptr mesh;
		std::vector<GLuint> frame_durations;

		unsigned int current_frame;
		unsigned int current_frame_end;
		bool reverse_looping;
		bool going_forward;
	public:
		Animation();
		bool load_from_file(Resource_manager& resource_manager, const std::string& name);
		void render_current(const glm::vec3& position);
		void reset_animation();
};

typedef std::shared_ptr<Animation> Animation_ptr;
/*=============================================*/

#endif