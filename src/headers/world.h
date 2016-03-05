#ifndef WORLD_H
#define WORLD_H


/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
#include "enum_light_type.h"
#include "contact.h"
#include "object.h"
#include "light.h"
#include "player.h"
#include "resource_manager.h"
#include "level.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <forward_list>
#include <list>
#include <vector>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class btDiscreteDynamicsWorld;

class World {
private:
	Level_ptr current_level;
	Resource_manager* manager;

	std::list<Object_ptr> players;
public:
	World(Resource_manager& init_manager);
	~World();

	bool add_player(const Object_ptr& player);
	bool add_players_to_physics_world();

	void render_world(Renderer& renderer);

	bool update_positions(float timedelta, Renderer& renderer);
	bool resolve_collisions();
	/*
	bool load_level();
	bool save_level();
	bool generate_level(){current_level->generate_level();};*/
};

/*
bool World::save_level(){
	std::ofstream contentf (current_level->get_name().c_str(), std::ios::binary | std::ios::ate | std::ios::app);

	contentf.write(reinterpret_cast<const char *>(&width), sizeof(GLuint));
	contentf.write(reinterpret_cast<const char *>(&height), sizeof(GLuint));
	contentf.write(reinterpret_cast<const char *>(&channels), sizeof(GLuint));
	contentf.write(reinterpret_cast<const char *>(&format), sizeof(GLint));
	contentf.write(reinterpret_cast<const char *>(&(image[0])), sizeof(unsigned char) * width * height * channels);

	contentf.close();

}*/
/*=============================================*/

#endif