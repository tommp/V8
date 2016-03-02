#include "world.h"

World::~World() {
}

World::World(Resource_manager& init_manager){
	manager = &init_manager;

	Character_ptr player = std::make_shared<Player>(init_manager);
	if (!add_player(player)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Could not add player to world" << std::endl;
		errorlogger("ERROR: Could not add player to world");
		exit(EXIT_FAILURE);
	}

	current_level = std::make_shared<Level>(init_manager);
	current_level->rendering_targets.push_back(&players);
	if (!add_players_to_physics_world()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Could not add player to physics world" << std::endl;
		errorlogger("ERROR: Could not add player to physics world");
		exit(EXIT_FAILURE);
	}
}

bool World::update_positions(GLfloat timedelta, Renderer& renderer){
	for (auto it = players.begin(); it != players.end(); ++it) {
		if (!(*it)->update_position(timedelta)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to update player position" << std::endl;
			errorlogger("ERROR: Failed to update player position");
			return false;
		}
	}

	if (!current_level->update_positions(timedelta, renderer)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to update object positions" << std::endl;
		errorlogger("ERROR: Failed to update object positions");
		return false;
	}

	/* TODO:: MOVE THIS */
	if(!players.empty()){
		if (!current_level->camera->center_camera(players.front())){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to center camera" << std::endl;
			errorlogger("ERROR: Failed to center camera");
			return false;
		}
	}
	return true;
}


void World::render_world(Renderer& renderer){
	current_level->render_level(renderer);
	renderer.present();
}

bool World::add_player(const Character_ptr& player){
	if (player){
		players.push_back(player);	
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null player in World" << std::endl;
		errorlogger("ERROR: Cannot add null player in World");
		return false;
	}
}
bool World::resolve_collisions(){
	if (!current_level->resolve_collisions()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to resolve collisions" << std::endl;
		errorlogger("ERROR: Failed to resolve collisions");
		return false;
	}
	return true;
}

bool World::add_players_to_physics_world(){
	for (auto player : players) {
		if (!current_level->add_to_physics_world(player->get_collision_body())){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Could not add player collision body" << std::endl;
			errorlogger("ERROR: Could not add player collision body");
			return false;
		}
	}
	return true;
}