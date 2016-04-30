#include "world.h"

World::~World() {
}

World::World(Resource_manager& init_manager, Renderer& renderer){
	manager = &init_manager;

	std::cout << "------------ Initializing world player..." << std::endl;
	Player_ptr player = std::make_shared<Player>(init_manager, "sphere_colored");
	if (!add_player(player)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Could not add player to world" << std::endl;
		errorlogger("FATAL ERROR: Could not add player to world");
		exit(EXIT_FAILURE);
	}
	std::cout << "------------ World player initialized!\n" << std::endl;

	std::cout << "------------ Initializing world starting level..." << std::endl;
	current_level = std::make_shared<Level>(init_manager, renderer);
	if (!add_players_to_physics_world()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Could not add player to physics world" << std::endl;
		errorlogger("FATAL ERROR: Could not add player to physics world");
		exit(EXIT_FAILURE);
	}
	std::cout << "------------ World starting level initialized!\n" << std::endl;

	std::cout << "------------ Initializing player rendering contexts..." << std::endl;
	if (!add_player_contexts_to_renderer(renderer)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Could not add player contexts to renderer!" << std::endl;
		errorlogger("FATAL ERROR: Could not add player contexts to renderer!");
		exit(EXIT_FAILURE);
	}
	std::cout << "------------ World player rendering contexts initialized!\n" << std::endl;
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
		if (!current_level->camera->center_camera(players.front()->get_position())) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to center camera" << std::endl;
			errorlogger("ERROR: Failed to center camera");
			return false;
		}
	}
	
	renderer.update_view_matrix(current_level->camera->get_position(), current_level->camera->get_target(), current_level->camera->get_up_dir());

	return true;
}


bool World::render_world(Renderer& renderer){
	if (!current_level->render_level(renderer)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render level!" << std::endl;
		errorlogger("ERROR: Failed to render level!");
		return false;
	}
	renderer.present();
	return true;
}

bool World::add_player(const Player_ptr& player){
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

bool World::add_players_to_physics_world()const{
	for (auto& player : players) {
		if (!current_level->add_to_physics_world(player->get_collision_body())){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Could not add player collision body" << std::endl;
			errorlogger("ERROR: Could not add player collision body");
			return false;
		}
	}
	return true;
}

bool World::add_player_contexts_to_renderer(Renderer& renderer)const{
	for (auto& player : players) {
		if (!player->add_contexts_to_renderer(renderer)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Could not add player contexts to renderer!" << std::endl;
			errorlogger("ERROR: Could not add player contexts to renderer!");
			return false;
		}
	}

	return true;
}