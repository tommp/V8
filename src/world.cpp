#include "./headers/world.h"

World::World(SDL_Renderer &ren, Resource_manager& manager){
	Player* new_player = new Player(ren, manager);
	add_player(new_player);
	current_level = new Level(10000, 10000, 10000);
	if(!players.empty()){
		current_level->center_camera(players.front());
	}
	for (int i = 0; i < 50000; i++) {
		Character* mob = new Slime_blob(ren, manager);
		add_character(mob);
	}
}

bool World::check_if_colliding(const Actor* a, const Actor* b)const{

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

bool World::check_if_colliding(const Actor* a, const SDL_Rect* b)const{

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

void World::detect_all_collisions() {
	detect_collisions(players);
	detect_collisions(players, characters);
	detect_collisions(players, props);
	detect_collisions(players, projectiles);

	if (!characters.empty()){
		detect_collisions(characters);
		detect_collisions(characters, props);
		detect_collisions(characters, projectiles);
	}

	if(!props.empty()){
		detect_collisions(props);
		detect_collisions(props, projectiles);
	}

	if(!projectiles.empty()){
		detect_collisions(projectiles);
	}
}

void World::detect_collisions(const std::forward_list<Player*>& a) {
	if (!a.empty()){
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
void World::detect_collisions(const std::forward_list<Player*>& a, const std::forward_list<Character*>& b) {
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
void World::detect_collisions(const std::forward_list<Player*>& a, const std::forward_list<Prop*>& b) {
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
void World::detect_collisions(const std::forward_list<Player*>& a, const std::forward_list<Projectile*>& b) {
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

void World::detect_collisions(const std::forward_list<Character*>& a){
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

void World::detect_collisions(const std::forward_list<Prop*>& a){
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

void World::detect_collisions(const std::forward_list<Projectile*>& a){
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


void World::detect_collisions(const std::forward_list<Character*>& a, const std::forward_list<Character*>& b){
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

void World::detect_collisions(const std::forward_list<Character*>& a, const std::forward_list<Prop*>& b){
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

void World::detect_collisions(const std::forward_list<Character*>& a, const std::forward_list<Projectile*>& b){
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

void World::detect_collisions(const std::forward_list<Prop*>& a, const std::forward_list<Prop*>& b){
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

void World::detect_collisions(const std::forward_list<Prop*>& a, const std::forward_list<Projectile*>& b){
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

void World::detect_collisions(const std::forward_list<Projectile*>& a, const std::forward_list<Projectile*>& b){
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
	auto prev_it_dormant = dormant_characters.before_begin();
	auto first_it_dormant = dormant_characters.begin();
	auto prev_it = characters.before_begin();

	for (auto it = characters.begin(); it != characters.end(); it++){
		if( !check_if_colliding(*it, current_level->get_camera_pointer())){
			add_dormant_character(*it);
			it = prev_it;
			characters.erase_after(it);
		}
		else{
			prev_it = it;
			continue;
		}
	}

	for (auto it = first_it_dormant; it != dormant_characters.end(); it++){
		if(check_if_colliding(*it, current_level->get_camera_pointer())){
			insert_character(*it);
			it = prev_it_dormant;
			characters.erase_after(it);
		}
		else{
			prev_it_dormant = it;
			continue;
		}
	}
}

void World::render_world(SDL_Renderer& ren){

	update_groups();

	auto player_it = players.begin();

	auto character_it = characters.begin();

	auto props_it = props.begin();

	auto proj_it = projectiles.begin();

	/* Render level */
	/* ====TODO==== */
	int smallest_z = 100000;
	int smallest_y = 100000;
	int smallest = -1;

	bool done = false;

	/* Render actors */
	while(!done){

		done = true;

		if(player_it != players.end()) {
			done = false;
			if( (*player_it)->get_z() < smallest_z ){
				smallest_z = (*player_it)->get_z();
				smallest_y = (*player_it)->get_y() + (*player_it)->get_height();
				smallest = 1;
			}
			else if( (*player_it)->get_z() == smallest_z ){
				if( ((*player_it)->get_y() + (*player_it)->get_height()) < smallest_y ){
					smallest_y = (*player_it)->get_y() + (*player_it)->get_height();
					smallest = 1;
				}
			}
		}

		if(character_it != characters.end()) {
			done = false;
			if( (*character_it)->get_z() < smallest_z ){
				smallest_z = (*character_it)->get_z();
				smallest_y = (*character_it)->get_y() + (*character_it)->get_height();
				smallest = 2;
			}
			else if( (*character_it)->get_z() == smallest_z ){
				if( ((*character_it)->get_y() + (*character_it)->get_height()) < smallest_y ){
					smallest_y = (*character_it)->get_y() + (*character_it)->get_height();
					smallest = 2;
				}
			}
		}

		if(props_it != props.end()) {
			done = false;
			if( (*props_it)->get_z() < smallest_z ){
				smallest_z = (*props_it)->get_z();
				smallest_y = (*props_it)->get_y() + (*props_it)->get_height();
				smallest = 3;
			}
			else if( (*props_it)->get_z() == smallest_z ){
				if( (*props_it)->get_y() + (*props_it)->get_height() < smallest_y ){
					smallest_y = (*props_it)->get_y() + (*props_it)->get_height();
					smallest = 3;
				}
			}
		}

		if(proj_it != projectiles.end()) {
			done = false;
			if( (*proj_it)->get_z() < smallest_z ){
				smallest_z = (*proj_it)->get_z();
				smallest_y = (*proj_it)->get_y() + (*proj_it)->get_height();
				smallest = 4;
			}
			else if( (*proj_it)->get_z() == smallest_z ){
				if( (*proj_it)->get_y() + (*proj_it)->get_height() < smallest_y ){
					smallest_y = (*proj_it)->get_y() + (*proj_it)->get_height();
					smallest = 4;
				}
			}
		}

		if (!done) {
			switch (smallest){
				case 1:
					(*player_it)->render_frame(ren, current_level->get_camera_pointer());
					player_it++;
					break;
				case 2:
					(*character_it)->render_frame(ren, current_level->get_camera_pointer());
					character_it++;
					break;
				case 3:
					(*props_it)->render_frame(ren, current_level->get_camera_pointer());
					props_it++;
					break;
				case 4:
					(*proj_it)->render_frame(ren, current_level->get_camera_pointer());
					proj_it++;
					break;
				default:
					break;
			}
		}
		smallest_z = 1000000;
		smallest_y = 1000000;
		smallest = -1;
	}
}

bool World::insert_character(Character* character){
	if (character){
		if (characters.empty()) {
			characters.push_front(character);
			return true;
		}
		else{
			auto last_pos_it = characters.before_begin();
			for (auto it = characters.begin(); it != characters.end(); it++) {
				if (character->get_z() == (*it)->get_z()) {
					if(character->get_y() <= (*it)->get_y()){
						characters.insert_after(last_pos_it, character);
						return true;
					}
					else{
						last_pos_it = it;
						continue;
					}
				}
				else if (character->get_z() < (*it)->get_z() ){
					characters.insert_after(last_pos_it, character);
					return true;
				}
				else{
					last_pos_it = it;
					continue;
				}
			}
			characters.insert_after(last_pos_it, character);
			return true;
		}
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

bool World::insert_player(Player* player){
	if (player){
		if (players.empty()) {
			players.push_front(player);
			return true;
		}
		else{
			auto last_pos_it = players.before_begin();
			for (auto it = players.begin(); it != players.end(); it++) {
				if (player->get_z() == (*it)->get_z()) {
					if(player->get_y() <= (*it)->get_y()){
						players.insert_after(last_pos_it, player);
						return true;
					}
					else{
						last_pos_it = it;
						continue;
					}
				}
				else if (player->get_z() < (*it)->get_z() ){
					players.insert_after(last_pos_it, player);
					return true;
				}
				else{
					last_pos_it = it;
					continue;
				}
			}
			players.insert_after(last_pos_it, player);
			return true;
		}
	}
	else{
		errorlogger("ERROR: Cannot add null player in World");
		std::cout << "ERROR: Cannot add null player in World" << std::endl;
		return false;
	}
}


bool World::add_player(Player* player){
	return insert_player(player);
}

bool World::insert_prop(Prop* prop){
	if (prop){
		if (props.empty()) {
			props.push_front(prop);
			return true;
		}
		else{
			auto last_pos_it = props.before_begin();
			for (auto it = props.begin(); it != props.end(); it++) {
				if (prop->get_z() == (*it)->get_z()) {
					if(prop->get_y() <= (*it)->get_y()){
						props.insert_after(last_pos_it, prop);
						return true;
					}
					else{
						last_pos_it = it;
						continue;
					}
				}
				else if (prop->get_z() < (*it)->get_z() ){
					props.insert_after(last_pos_it, prop);
					return true;
				}
				else{
					last_pos_it = it;
					continue;
				}
			}
			props.insert_after(last_pos_it, prop);
			return true;
		}
	}
	else{
		errorlogger("ERROR: Cannot add null prop in World");
		std::cout << "ERROR: Cannot add null prop in World" << std::endl;
		return false;
	}
}

bool World::add_dormant_prop(Prop* prop){
	dormant_props.push_front(prop);
	return true;
}

bool World::add_prop(Prop* prop){
	if(check_if_colliding(prop, current_level->get_camera_pointer())) {
		return insert_prop(prop);
	}
	else{
		return add_dormant_prop(prop);
	}
}

bool World::insert_projectile(Projectile* projectile){
	if (projectile){
		if (projectiles.empty()) {
			projectiles.push_front(projectile);
			return true;
		}
		else{
			auto last_pos_it = projectiles.before_begin();
			for (auto it = projectiles.begin(); it != projectiles.end(); it++) {
				if (projectile->get_z() == (*it)->get_z()) {
					if(projectile->get_y() <= (*it)->get_y()){
						projectiles.insert_after(last_pos_it, projectile);
						return true;
					}
					else{
						last_pos_it = it;
						continue;
					}
				}
				else if (projectile->get_z() < (*it)->get_z() ){
					projectiles.insert_after(last_pos_it, projectile);
					return true;
				}
				else{
					last_pos_it = it;
					continue;
				}
			}
			projectiles.insert_after(last_pos_it, projectile);
			return true;
		}
	}
	else{
		errorlogger("ERROR: Cannot add null projectile in World");
		std::cout << "ERROR: Cannot add null projectile in World" << std::endl;
		return false;
	}
}


bool World::add_projectile(Projectile* projectile){
	return insert_projectile(projectile);
}


void World::resolve_collisions(){
	contacts.clear();
}

Contact::Contact(Actor* c_a, Actor* c_b) {
	a = c_a;
	b = c_b;
}