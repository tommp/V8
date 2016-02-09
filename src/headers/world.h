#ifndef WORLD_H
#define WORLD_H


/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
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
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class World {
private:
	Level_ptr current_level;
	Resource_manager* manager;
	Shader_ptr light_shader;

	GLuint quad_VAO;

	std::list<Character_ptr> players;
	std::list<Character_ptr> characters;
	std::forward_list<Character_ptr> dormant_characters;
	std::forward_list<Contact> contacts;
	std::forward_list<Light_ptr> lights;
public:
	World(Resource_manager& init_manager);
	~World();
	bool check_if_colliding(const Character_ptr& a, const Character_ptr& b)const;
	bool check_if_offscreen(const Character_ptr& a)const;
	void update_positions(float timedelta);
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

	void render_quad()const;
	void render_world()const;
	void render_geometry()const;
	void render_lights()const;
};
/*=============================================*/

#endif