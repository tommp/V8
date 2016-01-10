#include "./headers/world.h"

World::World(Player* main_player){
	player = main_player;
}

bool World::check_if_colliding(Actor* a, Actor* b){

	if( ( a->get_y() + a->get_height() ) <= b->get_y() ){
		return false;
	}

	if( a->get_y() >= ( b->get_y() + b->get_height() ) ){
		return false;
	}

	if( ( a->get_x() + a->get_width() ) <= b->get_x() ){
		return false;
	}

	if( a->get_x() >= ( b->get_x() + b->get_width() ) ){
		return false;
	}

	if( ( a->get_z() + a->get_depth() ) <= b->get_z() ){
		return false;
	}

	if( a->get_z() >= ( b->get_z() + b->get_depth() ) ){
		return false;
	}

	/* If none of the sides from A are outside B */
	return true;
}

void World::update_positions(float timedelta){
	player->update_position(timedelta);
}

void World::detect_collisions(){

}

void World::resolve_collisions(){
	
}

void World::update_groups(){

}

void World::render_world(SDL_Renderer* ren){
	player->render_frame(ren);
}

Contact::Contact((Actor* c_a, Actor* c_b)) {
	a = c_a;
	b = c_b;
}