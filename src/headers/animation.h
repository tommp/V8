#ifndef ANIMATION_H
#define ANIMATION_H


/*Included headers*/
/*---------------------------------------------*/
#include "./errorlogger.h"
#include "./texture.h"
#include "./resource_manager.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <unistd.h>
#include <forward_list>
#include <fstream>
#include <sstream>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

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
		Animation(const Resource_manager& resource_manager, 
					const std::string& texture_name, 
					const std::string& filename);
		void reset_animation();
		void render_current(SDL_Renderer *ren, int x, int y);
		void render_texture(SDL_Renderer *ren, int x, int y);
};
/*=============================================*/

#endif