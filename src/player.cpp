#include "./headers/player.h"

Player::Player(SDL_Renderer& ren, Resource_manager& manager, Button_mappings& map){
	button_mappings = &map;
	std::string anim = "adventure_time";
	state = {"adv_down"};
	if ( !(animations = manager.load_animation_set(ren, anim) ) ){
		std::cout << "ERROR: Player constructor failed to load animation set: " << anim << std::endl;
		errorlogger("ERROR: Player constructor failed to load animation set: ", anim.c_str());
	}
	speed = 300;
	x = 2000;
	y = 2000;
	z = 0;
	width = TILESIZE;
	height = TILESIZE*2;
	depth = TILESIZE;
}

void Player::render_frame(SDL_Renderer &ren, SDL_Rect* offset){
	animations->render_current(ren, (int)x - offset->x, (int)y - offset->y, state);
}

void Player::update_position(float timedelta){
	int x_vec = 0;
	int y_vec = 0;

	const Uint8* current_key_states = SDL_GetKeyboardState(NULL);
	if(current_key_states[button_mappings->up]){
		y_vec -= 1;
	}
	if(current_key_states[button_mappings->left]){
		x_vec -= 1;
	}
	if(current_key_states[button_mappings->down]){
		y_vec += 1;
	}
	if(current_key_states[button_mappings->right]){
		x_vec += 1;	
	}

	x += x_vec * speed * timedelta;
	y += y_vec * speed * timedelta;

	if (x_vec > 0) {
		state = {"adv_right"};
	}
	else if (x_vec < 0){
		state = {"adv_left"};
	}
	else if (y_vec < 0){
		state = {"adv_up"};
	}
	else if (y_vec > 0){
		state = {"adv_down"};
	}
	else{
		if (state == "adv_right" || state == "adv_left" || state == "adv_up" || state == "adv_down") {
			state = state + "_loiter";
		}
	}
}

void Player::touch_character(Character& character){

}