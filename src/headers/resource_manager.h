#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H


/*Included headers*/
/*---------------------------------------------*/
#include "./texture.h"
#include "./errorlogger.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <unistd.h>
#include <unordered_map>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
typedef std::shared_ptr<Texture> Texture_ptr;

class Resource_manager {
	private:
		std::unordered_map<std::string, Texture_ptr> textures;
	public:
		Texture_ptr load_texture(SDL_Renderer *ren, const std::string& path, const std::string& name);
		Texture_ptr get_texture_ptr(const std::string& name)const {return textures.find(name)->second;};
};


/*=============================================*/

#endif

