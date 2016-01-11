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

	std::forward_list<Player*> players;

	std::forward_list<Character*> on_screen_characters;
	std::forward_list<Character*> off_screen_characters;
	std::forward_list<Character*> dormant_characters;

	std::forward_list<Prop*> on_screen_props;
	std::forward_list<Prop*> off_screen_props;
	std::forward_list<Prop*> dormant_props;

	std::forward_list<Projectile*> off_screen_projectiles;
	std::forward_list<Projectile*> on_screen_projectiles;

	std::forward_list<Contact> contacts;
public:
	World(SDL_Renderer &ren, Resource_manager& manager);
	bool check_if_colliding(const Actor* a, const Actor* b)const;
	void update_positions(float timedelta);

	void detect_all_collisions();
	void detect_collisions(const std::forward_list<Player*>& a);
	void detect_collisions(const std::forward_list<Player*>& a, const std::forward_list<Character*>& b);
	void detect_collisions(const std::forward_list<Player*>& a, const std::forward_list<Prop*>& b);
	void detect_collisions(const std::forward_list<Player*>& a, const std::forward_list<Projectile*>& b);
	void detect_collisions(const std::forward_list<Character*>& a);
	void detect_collisions(const std::forward_list<Character*>& a, const std::forward_list<Character*>& b);
	void detect_collisions(const std::forward_list<Character*>& a, const std::forward_list<Prop*>& b);
	void detect_collisions(const std::forward_list<Character*>& a, const std::forward_list<Projectile*>& b);
	void detect_collisions(const std::forward_list<Prop*>& a);
	void detect_collisions(const std::forward_list<Prop*>& a, const std::forward_list<Prop*>& b);
	void detect_collisions(const std::forward_list<Prop*>& a, const std::forward_list<Projectile*>& b);
	void detect_collisions(const std::forward_list<Projectile*>& a);
	void detect_collisions(const std::forward_list<Projectile*>& a, const std::forward_list<Projectile*>& b);
	
	void resolve_collisions();
	void update_groups();
	void render_world(SDL_Renderer& ren);
};
/*=============================================*/

#endif