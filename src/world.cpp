#include "world.h"

World::World(Resource_manager& init_manager){
	manager = &init_manager;
	current_level = std::make_shared<Level>(Level(4000, 4000, 200));
	Character* player = new Player(init_manager);
	add_player(player);

	
	for (int i = 0; i < 200; i++) {
		Character* cube = new Cube(init_manager);
		insert_character(cube);
	}
	
}

World::~World() {
	for (auto player : players) {
		delete player;
	}
}

bool World::check_if_colliding(const Character* a, const Character* b)const{

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

bool World::check_if_offscreen(const Character* a)const{
	return false;
}

void World::update_positions(GLfloat timedelta){
	
	for (auto it = players.begin(); it != players.end(); it++) {
		(*it)->update_position(timedelta);
	}
	for (auto it = characters.begin(); it != characters.end(); it++) {
		(*it)->update_position(timedelta);
	}

	if(!players.empty()){
		current_level->center_camera(players.front());
	}
	current_level->get_camera_pointer()->update_view_matrix();
	current_level->get_camera_pointer()->upload_view_matrix(manager->get_uniform_buffer("matrices"));
}

void World::sort_group(std::list<Character*>& list){
	for (auto it = list.begin(); it != list.end(); it++) {
		auto it2 = it;
		Character* temp;

		while( ( it2 != list.begin() ) && ( (**it2) < (**std::prev(it2)) ) ){
			temp = *it2;
			auto prev = std::prev(it2);
			(*it2) = (*prev);
			(*prev) = temp;
			it2--;
		}
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

void World::detect_collisions(const std::list<Character*>& a){
	if (!a.empty()) {
		for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
			for (auto it_a_2 = it_a; it_a_2 != a.end(); it_a_2++) {
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

void World::detect_collisions(const std::list<Character*>& a, const std::list<Character*>& b){
	for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
		for (auto it_b = b.begin(); it_b != b.end(); it_b++) {
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

	for (auto it = characters.begin(); it != characters.end(); it++){
		if( !check_if_offscreen(*it)){
			add_dormant_character(*it);
			it = characters.erase(it);
		}
	}

	for (auto it = first_it_dormant; it != dormant_characters.end(); it++){
		if(check_if_offscreen(*it)){
			insert_character(*it);
			dormant_characters.erase_after(before_it_dormant);
			it = before_it_dormant;
		}
		else{
			before_it_dormant++;
		}
	}
}



void World::render_world()const{
	for (auto player : players) {
		player->render_frame();
	}

	for (auto character : characters) {
		character->render_frame();
	}
}

bool World::insert_character(Character* character){
	if (character){
		characters.push_back(character);
		return true;
	}
	else{
		errorlogger("ERROR: Cannot add null Character in world");
		std::cout << "ERROR: Cannot add null Character in world" << std::endl;
		return false;
	}
}

bool World::add_dormant_character(Character* character){
	if(character){
		dormant_characters.push_front(character);
		return true;
	}
	else{
		errorlogger("ERROR: Cannot add null character in world");
		std::cout << "ERROR: Cannot add null character in world" << std::endl;
		return false;
	}
}

bool World::add_character(Character* character){
	if(character) {
		if(check_if_offscreen(character)) {
			return insert_character(character);
		}
		else{
			return add_dormant_character(character);
		}
	}
	else{
		errorlogger("ERROR: Cannot add null character in world");
		std::cout << "ERROR: Cannot add null character in world" << std::endl;
		return false;
	}
}

bool World::insert_player(Character* player){
	if (player){
		players.push_back(player);	
		return true;
	}
	else{
		errorlogger("ERROR: Cannot add null player in World");
		std::cout << "ERROR: Cannot add null player in World" << std::endl;
		return false;
	}
}


bool World::add_player(Character* player){
	return insert_player(player);
}

void World::resolve_collisions(){
	contacts.clear();
}