#ifndef WORLD_H
#define WORLD_H


/*Included headers*/
/*---------------------------------------------*/

#include "./errorlogger.h"
#include "./button_mappings.h"
#include "./contact.h"
#include "./mobs.h"
#include "./player.h"
#include "./resource_manager.h"
#include "./level.h"
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
	Level* current_level;
	std::list<Character*> players;
	std::list<Character*> characters;
	std::forward_list<Character*> dormant_characters;
	std::forward_list<Contact> contacts;
public:
	World(SDL_Renderer &ren, Resource_manager& manager, Button_mappings& map);
	bool check_if_colliding(const Character* a, const Character* b)const;
	bool check_if_colliding(const Character* a, const SDL_Rect* b)const;
	void update_positions(float timedelta);

	/* Sort group using insertion sort (since it will be nearly sorted most of the time) */
	void sort_group(std::list<Character*>& list);

	void detect_all_collisions();
	void detect_collisions(const std::list<Character*>& a);
	void detect_collisions(const std::list<Character*>& a, const std::list<Character*>& b);

	bool insert_player(Character* player);
	bool add_player(Character* player);
	
	bool insert_character(Character* character);
	bool add_dormant_character(Character* character);
	bool add_character(Character* character);

	void resolve_collisions();
	void update_groups();
	void render_world(SDL_Renderer& ren);
};
/*=============================================*/

#endif