#ifndef ANIMATION_H
#define ANIMATION_H


/*Included headers*/
/*---------------------------------------------*/
#include "./errorlogger.h"
#include "./frame.h"
#include "./utility.h"
#include "./texture.h"
#include "./resource_manager.h"
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
#define ANIMATION_DATA_FILE_PATH "../data/anim.boogie"

class Resource_manager;

class Animation{
	private:
		Texture_ptr texture;
		GLuint* VBO_set;
		GLuint* VAO_set;
		std::vector<GLuint> frame_durations;
		GLuint set_size;


		unsigned int current_frame;
		unsigned int current_frame_end;
		bool reverse_looping;
		bool going_forward;
	public:
		Animation();
		bool load_from_file(Resource_manager& resource_manager, const std::string& name);
		void render_current();
		void reset_animation();
};

typedef std::shared_ptr<Animation> Animation_ptr;
/*=============================================*/

#endif