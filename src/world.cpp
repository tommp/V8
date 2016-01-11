#include "./headers/world.h"

World::World(SDL_Renderer &ren, Resource_manager& manager){
	Player_ptr new_player = std::make_shared<Player>(ren, manager);
	players.push_front(new_player);
}

bool World::check_if_colliding(const Actor& a, const Actor& b)const{

	if( ( a.get_y() + a.get_height() ) <= b.get_y() ){
		return false;
	}

	if( a.get_y() >= ( b.get_y() + b.get_height() ) ){
		return false;
	}

	if( ( a.get_x() + a.get_width() ) <= b.get_x() ){
		return false;
	}

	if( a.get_x() >= ( b.get_x() + b.get_width() ) ){
		return false;
	}

	if( ( a.get_z() + a.get_depth() ) <= b.get_z() ){
		return false;
	}

	if( a.get_z() >= ( b.get_z() + b.get_depth() ) ){
		return false;
	}

	/* If none of the sides from A are outside B */
	return true;
}

void World::update_positions(float timedelta){
	for (auto it = players.begin(); it != players.end(); it++) {
		it->get()->update_position(timedelta);
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

void World::detect_collisions(const std::forward_list<Player_ptr>& a) {
	if( !(a.begin() != a.end() )){
		for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
			for (auto it_a_2 = ++it_a; it_a_2 != a.end(); it_a_2++) {
				debug("lp2");
				if(check_if_colliding( *(it_a->get()), *(it_a_2->get()) )){
					if (it_a == it_a_2) {
						continue;
					}
					else{
						contacts.emplace_front( *(it_a->get()), *(it_a_2->get()) );
					}
				}
				else{
					continue;
				}
			}
		}
	}
}
void World::detect_collisions(const std::forward_list<Player_ptr>& a, const std::forward_list<Character_ptr>& b) {
	for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
		for (auto it_b = b.begin(); it_b != b.end(); it_b++) {
			if(check_if_colliding( *(it_a->get()), *(it_b->get()) )){
				contacts.emplace_front( *(it_a->get()), *(it_b->get()) );
			}
			else{
				continue;
			}
		}
	}
}
void World::detect_collisions(const std::forward_list<Player_ptr>& a, const std::forward_list<Prop_ptr>& b) {
	for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
		for (auto it_b = b.begin(); it_b != b.end(); it_b++) {
			if(check_if_colliding( *(it_a->get()), *(it_b->get()) )){
				contacts.emplace_front( *(it_a->get()), *(it_b->get()) );
			}
			else{
				continue;
			}
		}
	}
}
void World::detect_collisions(const std::forward_list<Player_ptr>& a, const std::forward_list<Projectile_ptr>& b) {
	for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
		for (auto it_b = b.begin(); it_b != b.end(); it_b++) {
			if(check_if_colliding( *(it_a->get()), *(it_b->get()) )){
				contacts.emplace_front( *(it_a->get()), *(it_b->get()) );
			}
			else{
				continue;
			}
		}
	}
}

void World::detect_collisions(const std::forward_list<Character_ptr>& a){
	if( !(a.begin() != a.end() )){
		if( !(a.begin() != a.end() )){
			for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
				for (auto it_a_2 = ++it_a; it_a_2 != a.end(); it_a_2++) {
					if(check_if_colliding( *(it_a->get()), *(it_a_2->get()) )){
						if (it_a == it_a_2) {
							continue;
						}
						else{
							contacts.emplace_front( *(it_a->get()), *(it_a_2->get()) );
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

void World::detect_collisions(const std::forward_list<Prop_ptr>& a){
	if( !(a.begin() != a.end() )){
		for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
			for (auto it_a_2 = ++it_a; it_a_2 != a.end(); it_a_2++) {
				if(check_if_colliding( *(it_a->get()), *(it_a_2->get()) )){
					contacts.emplace_front( *(it_a->get()), *(it_a_2->get()) );
				}
				else{
					continue;
				}
			}
		}
	}
}

void World::detect_collisions(const std::forward_list<Projectile_ptr>& a){
	for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
		for (auto it_a_2 = ++it_a; it_a_2 != a.end(); it_a_2++) {
			if(check_if_colliding( *(it_a->get()), *(it_a_2->get()) )){
				contacts.emplace_front( *(it_a->get()), *(it_a_2->get()) );
			}
			else{
				continue;
			}
		}
	}
}


void World::detect_collisions(const std::forward_list<Character_ptr>& a, const std::forward_list<Character_ptr>& b){
	for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
		for (auto it_b = b.begin(); it_b != b.end(); it_b++) {
			if(check_if_colliding( *(it_a->get()), *(it_b->get()) )){
				contacts.emplace_front( *(it_a->get()), *(it_b->get()) );
			}
			else{
				continue;
			}
		}
	}
}

void World::detect_collisions(const std::forward_list<Character_ptr>& a, const std::forward_list<Prop_ptr>& b){
	for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
		for (auto it_b = b.begin(); it_b != b.end(); it_b++) {
			if(check_if_colliding( *(it_a->get()), *(it_b->get()) )){
				contacts.emplace_front( *(it_a->get()), *(it_b->get()) );
			}
			else{
				continue;
			}
		}
	}
}

void World::detect_collisions(const std::forward_list<Character_ptr>& a, const std::forward_list<Projectile_ptr>& b){
	for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
		for (auto it_b = b.begin(); it_b != b.end(); it_b++) {
			if(check_if_colliding( *(it_a->get()), *(it_b->get()) )){
				contacts.emplace_front( *(it_a->get()), *(it_b->get()) );
			}
			else{
				continue;
			}
		}
	}
}

void World::detect_collisions(const std::forward_list<Prop_ptr>& a, const std::forward_list<Prop_ptr>& b){
	for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
		for (auto it_b = b.begin(); it_b != b.end(); it_b++) {
			if(check_if_colliding( *(it_a->get()), *(it_b->get()) )){
				contacts.emplace_front( *(it_a->get()), *(it_b->get()) );
			}
			else{
				continue;
			}
		}
	}
}

void World::detect_collisions(const std::forward_list<Prop_ptr>& a, const std::forward_list<Projectile_ptr>& b){
	for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
		for (auto it_b = b.begin(); it_b != b.end(); it_b++) {
			if(check_if_colliding( *(it_a->get()), *(it_b->get()) )){
				contacts.emplace_front( *(it_a->get()), *(it_b->get()) );
			}
			else{
				continue;
			}
		}
	}
}

void World::detect_collisions(const std::forward_list<Projectile_ptr>& a, const std::forward_list<Projectile_ptr>& b){
	for (auto it_a = a.begin(); it_a != a.end(); it_a++) {
		for (auto it_b = b.begin(); it_b != b.end(); it_b++) {
			if(check_if_colliding( *(it_a->get()), *(it_b->get()) )){
				contacts.emplace_front( *(it_a->get()), *(it_b->get()) );
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
	for (auto it = players.begin(); it != players.end(); it++) {
		it->get()->render_frame(ren);
	}
}

Contact::Contact(Actor& c_a, Actor& c_b) {
	a = &c_a;
	b = &c_b;
}