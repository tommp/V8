#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H


/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
#include "utility.h"
#include "texture.h"
#include "mesh.h"
#include "animation.h"
#include "animation_set.h"
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
class Animation_set;
typedef std::shared_ptr<Animation_set> Animation_set_ptr;

class Resource_manager {
	private:
		std::unordered_map<std::string, Texture_ptr> textures;
		std::unordered_map<std::string, Animation_ptr> animations;
		std::unordered_map<std::string, Mesh_ptr> meshes;
	public:
		Texture_ptr load_texture(const std::string& name);
		Animation_ptr load_animation(const std::string& name);
		Mesh_ptr load_mesh(const std::string& name);
		Animation_set_ptr load_animation_set(const std::string& name);
};
/*=============================================*/

#endif

