#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H


/*Included headers*/
/*---------------------------------------------*/
#include "./texture.h"
#include "./animation.h"
#include "./errorlogger.h"
#include "./utility.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <unistd.h>
#include <unordered_map>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Animation;
typedef std::shared_ptr<Animation> Animation_ptr;

class Resource_manager {
	private:
		std::unordered_map<std::string, Texture_ptr> textures;
		std::unordered_map<std::string, Animation_ptr> animations;
	public:
		Texture_ptr load_texture(SDL_Renderer& ren, const std::string& name);
		Animation_ptr load_animation(SDL_Renderer& ren, const std::string& name);
};
/*=============================================*/

#endif

