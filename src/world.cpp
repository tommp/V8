#include "./headers/world.h"

World::World(SDL_Renderer &ren, Resource_manager& manager){
	Player* new_player = new Player(ren, manager);
	for (int i = 0; i < 500; i++) {
		Character* mob = new Slime_blob(ren, manager);
		on_screen_characters.push_front(mob);
	}
	players.push_front(new_player);
}

bool World::check_if_colliding(const Actor* a, const Actor* b)const{

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
	for (auto it = players.begin(); it != players.end(); it++) {
		(*it)->update_position(timedelta);
	}
	for (auto it = on_screen_characters.begin(); it != on_screen_characters.end(); it++) {
		(*it)->update_position(timedelta);
	}
}

void World::detect_all_collisions() {
	detect_collisions(players);
	detect_collisions(players, on_screen_characters);
	detect_collisions(players, off_screen_characters);
	detect_collisions(players, on_screen_props);
	detect_collisions(players, off_screen_props);
	detect_collisions(players, on_screen_projectiles);
	detect_collisions(players, off_screen_projectiles);

	if (!on_screen_characters.empty()){
		detect_collisions(on_screen_characters);
		detect_collisions(on_screen_characters, off_screen_characters);
		detect_collisions(on_screen_characters, on_screen_props);
		detect_collisions(on_screen_characters, off_screen_props);
		detect_collisions(on_screen_characters, on_screen_projectiles);
		detect_collisions(on_screen_characters, off_screen_projectiles);
	}

	if (!off_screen_characters.empty()){
		detect_collisions(off_screen_characters);
		detect_collisions(off_screen_characters, on_screen_props);
		detect_collisions(off_screen_characters, off_screen_props);
		detect_collisions(off_screen_characters, on_screen_projectiles);
		detect_collisions(off_screen_characters, off_screen_projectiles);
	}

	if(!on_screen_props.empty()){
		detect_collisions(on_screen_props);
		detect_collisions(on_screen_props, off_screen_props);
		detect_collisions(on_screen_props, on_screen_projectiles);
		detect_collisions(on_screen_props, off_screen_projectiles);
	}

	if(!off_screen_props.empty()){
		detect_collisions(off_screen_props);
		detect_collisions(off_screen_props, on_screen_projectiles);
		detect_collisions(off_screen_props, off_screen_projectiles);
	}

	if(!on_screen_projectiles.empty()){
		detect_collisions(on_screen_projectiles);
		detect_collisions(on_screen_projectiles, off_screen_projectiles);
	}

	if(!off_screen_projectiles.empty()){
		detect_collisions(off_screen_projectiles);
	}

}

void World::detect_collisions(const std::forward_list<Player*>& a) {
	if( !(a.begin() != a.end() )){
		for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
			for (auto it_a_2 = ++it_a; it_a_2 != a.end(); it_a_2++) {
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
	if( !(a.begin() != a.end() )){
		if( !(a.begin() != a.end() )){
			for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
				for (auto it_a_2 = ++it_a; it_a_2 != a.end(); it_a_2++) {
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
}

void World::detect_collisions(const std::forward_list<Prop*>& a){
	if( !(a.begin() != a.end() )){
		for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
			for (auto it_a_2 = ++it_a; it_a_2 != a.end(); it_a_2++) {
				if(check_if_colliding( *(it_a), *(it_a_2) )){
					contacts.emplace_front( *(it_a), *(it_a_2) );
				}
				else{
					continue;
				}
			}
		}
	}
}

void World::detect_collisions(const std::forward_list<Projectile*>& a){
	for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
		for (auto it_a_2 = ++it_a; it_a_2 != a.end(); it_a_2++) {
			if(check_if_colliding( *(it_a), *(it_a_2) )){
				contacts.emplace_front( *(it_a), *(it_a_2) );
			}
			else{
				continue;
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

void World::resolve_collisions(){
	
}

void World::update_groups(){

}

void World::render_world(SDL_Renderer& ren){
	auto player_it = players.begin();

	auto character_it = on_screen_characters.begin();

	auto props_it = on_screen_props.begin();

	auto proj_it = on_screen_projectiles.begin();

	/* Render level */
	/* ====TODO==== */
	int biggest_z = -1;
	int biggest_y = -1;
	int biggest = 1;

	bool done = false;

	/* Render actors */
	while(!done){

		done = true;

		if(player_it != players.end()) {
			done = false;
			if( (*player_it)->get_z() > biggest_z ){
				biggest_z = (*player_it)->get_z();
				biggest = 1;
			}
			else if( (*player_it)->get_z() == biggest_z ){
				if( (*player_it)->get_y() > biggest_y ){
					biggest_y = (*player_it)->get_y();
					biggest = 1;
				}
			}
		}

		if(character_it != on_screen_characters.end()) {
			done = false;
			if( (*character_it)->get_z() > biggest_z ){
				biggest_z = (*character_it)->get_z();
				biggest = 2;
			}
			else if( (*character_it)->get_z() == biggest_z ){
				if( (*character_it)->get_y() > biggest_y ){
					biggest_y = (*character_it)->get_y();
					biggest = 2;
				}
			}
		}

		if(props_it != on_screen_props.end()) {
			done = false;
			if( (*props_it)->get_z() > biggest_z ){
				biggest_z = (*props_it)->get_z();
				biggest = 3;
			}
			else if( (*props_it)->get_z() == biggest_z ){
				if( (*props_it)->get_y() > biggest_y ){
					biggest_y = (*props_it)->get_y();
					biggest = 3;
				}
			}
		}

		if(proj_it != on_screen_projectiles.end()) {
			done = false;
			if( (*proj_it)->get_z() > biggest_z ){
				biggest_z = (*proj_it)->get_z();
				biggest = 4;
			}
			else if( (*proj_it)->get_z() == biggest_z ){
				if( (*proj_it)->get_y() > biggest_y ){
					biggest_y = (*proj_it)->get_y();
					biggest = 4;
				}
			}
		}

		if (!done) {
			switch (biggest){
				case 1:
					(*player_it)->render_frame(ren);
					player_it++;
					break;
				case 2:
					(*character_it)->render_frame(ren);
					character_it++;
					break;
				case 3:
					(*props_it)->render_frame(ren);
					props_it++;
					break;
				case 4:
					(*proj_it)->render_frame(ren);
					proj_it++;
					break;
				default:
					break;
			}
			biggest_z = -1;
			biggest_y = -1;
			biggest = -1;
		}

	}

}

Contact::Contact(Actor* c_a, Actor* c_b) {
	a = c_a;
	b = c_b;
}