#ifndef WORLD_H
#define WORLD_H


/*Included headers*/
/*---------------------------------------------*/
#include "./actor.h"
#include "./player.h"
#include "./level.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class World {
private:
	Player* player;
	Level_ptr current_level;

	std::forward_list<Character_ptr> on_screen_characters;
	std::forward_list<Character_ptr> off_screen_characters;
	std::forward_list<Character_ptr> dormant_characters;

	std::forward_list<Prop_ptr> on_screen_props;
	std::forward_list<Prop_ptr> off_screen_props;
	std::forward_list<Prop_ptr> dormant_props;

	std::forward_list<Projectile_ptr> off_screen_projectiles;
	std::forward_list<Projectile_ptr> on_screen_projectiles;

	std::forward_list<Contact> contacts;
public:
	World(Player* main_player);
	bool check_if_colliding(Actor* a, Actor* b);
	void update_positions(float timedelta);
	void detect_collisions();
	void resolve_collisions();
	void update_groups();
	void render_world(SDL_Renderer* ren);
};

class Contact {
private:
	Actor* a;
	Actor* b;
public:
	Contact(Actor* c_a, Actor* c_b);
};


/*=============================================*/

#endif