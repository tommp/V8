#ifndef WORLD_H
#define WORLD_H


/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
#include "enum_light_type.h"
#include "contact.h"
#include "character.h"
#include "light.h"
#include "mobs.h"
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
class World {
private:
	Level_ptr current_level;
	Resource_manager* manager;

	Camera_ptr camera;

	std::vector<const std::list<Character_ptr>*> rendering_targets;

	std::list<Character_ptr> players;
	std::list<Character_ptr> characters;
	std::forward_list<Character_ptr> dormant_characters;
	std::forward_list<Contact> contacts;
	
	std::forward_list<Light_ptr> dir_lights;
	std::forward_list<Light_ptr> point_lights;
	std::forward_list<Light_ptr> spot_lights;
public:
	World(Resource_manager& init_manager);
	~World();
	bool check_if_colliding(const Character_ptr& a, const Character_ptr& b)const;
	bool check_if_offscreen(const Character_ptr& a)const;
	void update_positions(float timedelta, Renderer& renderer);
	void resolve_collisions();
	void update_groups();

	void detect_all_collisions();
	void detect_collisions(const std::list<Character_ptr>& a);
	void detect_collisions(const std::list<Character_ptr>& a, const std::list<Character_ptr>& b);

	bool insert_player(const Character_ptr& player);
	bool add_player(const Character_ptr& player);
	
	bool insert_character(const Character_ptr& character);
	bool add_dormant_character(const Character_ptr& character);
	bool add_character(const Character_ptr& character);

	bool add_dir_light(const Light_ptr& light);
	bool add_point_light(const Light_ptr& light);
	bool add_spot_light(const Light_ptr& light);


	void render_world(Renderer& renderer);
	void render_geometry(Renderer& renderer);
	void render_lights(const Renderer& renderer)const;
};
/*=============================================*/

#endif