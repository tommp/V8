#ifndef ANIMATION_SET_H
#define ANIMATION_SET_H


/*Included headers*/
/*---------------------------------------------*/
#include "./errorlogger.h"
#include "./utility.h"
#include "./resource_manager.h"
#include "./texture.h"
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