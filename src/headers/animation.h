#ifndef ANIMATION_H
#define ANIMATION_H


/*Included headers*/
/*---------------------------------------------*/
#include "./resource_manager.h"
#include "./errorlogger.h"
#include "./texture.h"
#include "./utility.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <unistd.h>
#include <forward_list>
#include <fstream>
#include <sstream>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

class Resource_manager;

class Frame{
	friend class Animation;
	private:
		/* Clipping rectangle for texture */
		SDL_Rect clip;
		/* duration in ms */
		int duration;
	public:
		Frame(int x, int y, int w, int h, int frame_duration);
};

class Animation{
	private:
		Texture_ptr texture;

		/* List of frames */
		std::forward_list<Frame> frames;
		std::forward_list<Frame>::iterator current_frame;
		unsigned int current_frame_end;
	public:
		Animation();
		bool load_from_file(SDL_Renderer& ren, Resource_manager& resource_manager, const std::string& name);
		void render_current(SDL_Renderer& ren, int x, int y);
		void render_texture(SDL_Renderer& ren, int x, int y);
		void reset_animation();
};

typedef std::shared_ptr<Animation> Animation_ptr;
/*=============================================*/

#endif