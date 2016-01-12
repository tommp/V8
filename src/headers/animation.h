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
#include <list>
#include <fstream>
#include <sstream>
#include <memory>
#include <unordered_map>
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
		std::list<Frame> frames;
		std::list<Frame>::iterator current_frame;
		unsigned int current_frame_end;
		bool reverse_looping;
		bool going_forward;
	public:
		Animation();
		bool load_from_file(SDL_Renderer& ren, Resource_manager& resource_manager, const std::string& name);
		void render_current(SDL_Renderer& ren, int x, int y);
		void reset_animation();
};

typedef std::shared_ptr<Animation> Animation_ptr;

class Animation_set {
private:
	std::unordered_map<std::string, Animation_ptr> animations;
public:
	Animation_set();
	void render_current(SDL_Renderer& ren, int x, int y, const std::string& name);
	bool load_from_file(SDL_Renderer& ren, Resource_manager& resource_manager, const std::string& name);
};

typedef std::shared_ptr<Animation_set> Animation_set_ptr;
/*=============================================*/

#endif