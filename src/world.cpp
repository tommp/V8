#include "./headers/world.h"

World::World(SDL_Renderer &ren, Resource_manager& manager, Button_mappings& map){
	Character* new_player = new Player(ren, manager, map);
	add_player(new_player);
	current_level = new Level(10000, 10000, 10000);
	if(!players.empty()){
		current_level->center_camera(players.front());
	}
	for (int i = 0; i < 20000; i++) {
		Character* mob = new Slime_blob(ren, manager);
		add_character(mob);
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

bool World::check_if_colliding(const Character* a, const SDL_Rect* b)const{

	if(! a || !b){
		std::cout << "WARNING: NULLPTR PASSED TO COLLISION CHECK FOR CAMERA!" << std::endl;
		return false;
	}

	if( ( a->get_y() + a->get_height() ) <= (b->y - RENDERING_SLACK) ){
		return false;
	}

	if( a->get_y() >= ( b->y + b->h + RENDERING_SLACK) ){
		return false;
	}

	if( ( a->get_x() + a->get_width() ) <= (b->x - RENDERING_SLACK) ){
		return false;
	}

	if( a->get_x() >= ( b->x + b->w  + RENDERING_SLACK) ){
		return false;
	}

	/* If none of the sides from A are outside B */
	return true;
}

void World::update_positions(float timedelta){

	for (auto it = players.begin(); it != players.end(); it++) {
		(*it)->update_position(timedelta);
	}
	for (auto it = characters.begin(); it != characters.end(); it++) {
		(*it)->update_position(timedelta);
	}

	if(!players.empty()){
		current_level->center_camera(players.front());
	}
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
		if( !check_if_colliding(*it, current_level->get_camera_pointer())){
			add_dormant_character(*it);
			it = characters.erase(it);
		}
	}

	for (auto it = first_it_dormant; it != dormant_characters.end(); it++){
		if(check_if_colliding(*it, current_level->get_camera_pointer())){
			insert_character(*it);
			dormant_characters.erase_after(before_it_dormant);
			it = before_it_dormant;
		}
		else{
			before_it_dormant++;
		}
	}
}

void World::render_world(SDL_Renderer& ren){
	update_groups();
	sort_group(players);
	sort_group(characters);
	auto player_it = players.begin();

	auto character_it = characters.begin();

	/* Render level */
	/* ====TODO==== */

	bool players_done = false;
	bool characters_done = false;

	/* Render characters */
	while( (!players_done) || (!characters_done)) {

		/* Dangerous but clean, works as long as compiler optimizes correctly */
		if ( (!players_done) && (**player_it < **character_it) ) {
			(*player_it)->render_frame(ren, current_level->get_camera_pointer());
			player_it++;

			if (player_it == players.end()) {
				players_done = true;
			}
		}
		else if ( !characters_done ) {
			(*character_it)->render_frame(ren, current_level->get_camera_pointer());
			character_it++;
			if (character_it == characters.end()) {
				characters_done = true;
			}
		}
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
		if(check_if_colliding(character, current_level->get_camera_pointer())) {
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