#include "headers/player.h"

Player::Player(Resource_manager& manager, Button_mappings& map){
	button_mappings = &map;
	speed = 300;
	position[0] = 2000;
	position[1] = 2000;
	position[2] = 0;
	width = TILESIZE;
	height = TILESIZE*2;
	depth = TILESIZE;
}

void Player::render_frame(const glm::vec3& position){
	animations->render_current(state, position);
}

void Player::update_position(float timedelta){
	const Uint8* current_key_states = SDL_GetKeyboardState(NULL);
	if(current_key_states[button_mappings->up]){
		velocity[1] -= 1;
	}
	if(current_key_states[button_mappings->left]){
		velocity[0] -= 1;
	}
	if(current_key_states[button_mappings->down]){
		velocity[1] += 1;
	}
	if(current_key_states[button_mappings->right]){
		velocity[0] += 1;	
	}

	position[0] += velocity[0] * speed * timedelta;
	position[1] += velocity[1] * speed * timedelta;

	if (velocity[0] > 0) {
		state = {"adv_right"};
	}
	else if (velocity[0] < 0){
		state = {"adv_left"};
	}
	else if (velocity[1] < 0){
		state = {"adv_up"};
	}
	else if (velocity[1] > 0){
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