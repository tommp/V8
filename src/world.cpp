#include "world.h"

World::World(Resource_manager& init_manager){
	manager = &init_manager;
	current_level = std::make_shared<Level>(Level(4000, 4000, 200));
	Character_ptr player = std::make_shared<Player>(init_manager);
	add_player(player);

	
	for (int i = 0; i < 50; ++i) {
		Character_ptr cube = std::make_shared<Cube>(init_manager);
		insert_character(cube);
	}
	
	
	for (int i = 0; i < 50; ++i) {
		Light_ptr point_light = std::make_shared<Point_light>();
		add_point_light(point_light);
	}

	Light_ptr dir_light = std::make_shared<Directional_light>();
	add_dir_light(dir_light);
}

World::~World() {
}

bool World::check_if_colliding(const Character_ptr& a, const Character_ptr& b)const{

	if(!a || !b){
		return false;
	}

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

bool World::check_if_offscreen(const Character_ptr& a)const{
	return false;
}

void World::update_positions(GLfloat timedelta, Renderer& renderer){
	
	for (auto it = players.begin(); it != players.end(); ++it) {
		(*it)->update_position(timedelta);
	}
	for (auto it = characters.begin(); it != characters.end(); ++it) {
		(*it)->update_position(timedelta);
	}

	if(!players.empty()){
		renderer.center_camera(players.front(), current_level->get_width(), current_level->get_height());
	}
}


void World::detect_all_collisions() {
	if (!players.empty()){
		detect_collisions(players);
		detect_collisions(players, characters);
	}

	if (!characters.empty()){
		detect_collisions(characters);
	}
}

void World::detect_collisions(const std::list<Character_ptr>& a){
	if (!a.empty()) {
		for (auto it_a = a.begin(); it_a != a.end(); ++it_a) {
			for (auto it_a_2 = it_a; it_a_2 != a.end(); ++it_a_2) {
				if(check_if_colliding( *(it_a), *(it_a_2) )){
					if (it_a == it_a_2) {
						continue;
					}
					else{
						contacts.emplace_front( *(it_a), *(it_a_2) );
					}
				}
				else{
					continue;
				}
			}
		}
	}
}

void World::detect_collisions(const std::list<Character_ptr>& a, const std::list<Character_ptr>& b){
	for (auto it_a = a.begin(); it_a != a.end(); ++it_a) {
		for (auto it_b = b.begin(); it_b != b.end(); ++it_b) {
			if(check_if_colliding( *(it_a), *(it_b) )){
				contacts.emplace_front( *(it_a), *(it_b) );
			}
			else{
				continue;
			}
		}
	}
}

void World::update_groups(){
	auto first_it_dormant = dormant_characters.begin();
	auto before_it_dormant = dormant_characters.before_begin();

	for (auto it = characters.begin(); it != characters.end(); ++it){
		if( !check_if_offscreen(*it)){
			add_dormant_character(*it);
			it = characters.erase(it);
		}
	}

	for (auto it = first_it_dormant; it != dormant_characters.end(); ++it){
		if(check_if_offscreen(*it)){
			insert_character(*it);
			dormant_characters.erase_after(before_it_dormant);
			it = before_it_dormant;
		}
		else{
			++before_it_dormant;
		}
	}
}

void World::render_geometry(Renderer& renderer){
	renderer.setup_geometry_rendering();

	for (auto player : players) {
		player->render_frame(renderer);
	}

	for (auto character : characters) {
		character->render_frame(renderer);
	}

	renderer.detach_geometry_rendering();

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render geometry!"<< std::endl;
		errorlogger("ERROR: Failed to render geometry!");
		exit(EXIT_FAILURE);
	}
}

void World::render_lights(const Renderer& renderer)const{
	renderer.setup_light_rendering();
	renderer.bind_g_data(renderer.get_light_shader(2));
	renderer.upload_view_position(renderer.get_light_shader_program(2));

	
	for (auto light : dir_lights) {
		light->render_light(renderer);
	}
	
	renderer.bind_g_data(renderer.get_light_shader(0));
	renderer.upload_view_position(renderer.get_light_shader_program(0));

	for (auto light : point_lights) {
		light->render_light(renderer);
	}

	renderer.detach_light_rendering();
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render lights!" << std::endl;
		errorlogger("ERROR: Failed to render lights!");
		exit(EXIT_FAILURE);
	}
}


void World::render_world(Renderer& renderer){
	render_geometry(renderer);
	render_lights(renderer);
	renderer.present_display();
}

bool World::insert_character(const Character_ptr& character){
	if (character){
		characters.push_back(character);
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null Character in world" << std::endl;
		errorlogger("ERROR: Cannot add null Character in world");
		return false;
	}
}

bool World::add_dormant_character(const Character_ptr& character){
	if(character){
		dormant_characters.push_front(character);
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null character in world" << std::endl;
		errorlogger("ERROR: Cannot add null character in world");
		return false;
	}
}

bool World::add_character(const Character_ptr& character){
	if(character) {
		if(check_if_offscreen(character)) {
			return insert_character(character);
		}
		else{
			return add_dormant_character(character);
		}
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null character in world" << std::endl;
		errorlogger("ERROR: Cannot add null character in world");
		return false;
	}
}

bool World::insert_player(const Character_ptr& player){
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

bool World::add_dir_light(const Light_ptr& light){
	if (light){
		dir_lights.push_front(light);	
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null directional light in World" << std::endl;
		errorlogger("ERROR: Cannot add null directional light in World");
		return false;
	}
}

bool World::add_point_light(const Light_ptr& light){
	if (light){
		point_lights.push_front(light);	
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null point light in World" << std::endl;
		errorlogger("ERROR: Cannot add null point light in World");
		return false;
	}
}

bool World::add_spot_light(const Light_ptr& light){
	if (light){
		spot_lights.push_front(light);	
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add null point light in World" << std::endl;
		errorlogger("ERROR: Cannot add null point light in World");
		return false;
	}
}


bool World::add_player(const Character_ptr& player){
	return insert_player(player);
}

void World::resolve_collisions(){
	contacts.clear();
}