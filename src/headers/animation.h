#ifndef ANIMATION_H
#define ANIMATION_H


/*Included headers*/
/*---------------------------------------------*/
#include "./errorlogger.h"
#include "./texture.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <unistd.h>
#include <forward_list>
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
		std::string name;
		Texture* texture;

		/* List of frames */
		std::forward_list<Frame> frames;
		std::forward_list<Frame>::iterator current_frame;
		unsigned int current_frame_end;
	public:
		Animation();
		void reset_animation(){current_frame = frames.begin();};
		void render_current(SDL_Renderer *ren, int x, int y);
};
/*=============================================*/

#endif