#ifndef WORLD_H
#define WORLD_H


/*Included headers*/
/*---------------------------------------------*/
#include "./level.h"
#include "./actor.h"
#include "./player.h"
#include "./mobs.h"
#include "./resource_manager.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <forward_list>
#include <list>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Contact{
private:
	Actor* a;
	Actor* b;
public:
	Contact(Actor* c_a, Actor* c_b);
};

class World {
private:
	Level* current_level;
	std::list<Character*> players;
	std::list<Character*> characters;
	std::forward_list<Character*> dormant_characters;
	std::forward_list<Contact> contacts;
public:
	World(SDL_Renderer &ren, Resource_manager& manager);
	bool check_if_colliding(const Actor* a, const Actor* b)const;
	bool check_if_colliding(const Actor* a, const SDL_Rect* b)const;
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