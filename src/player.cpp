#include "./headers/player.h"

Player::Player(Animation* init_animation){
	animation = init_animation;
	speed = 300;
	x = 100;
	y = 100;
	z = 0;
}

void Player::render_frame(SDL_Renderer *ren){
	animation->render_current(ren, (int)x, (int)y);
}

void Player::update_position(float timedelta){
	int x_vec = 0;
	int y_vec = 0;

	const Uint8* current_key_states = SDL_GetKeyboardState(NULL);
	if(current_key_states[SDL_SCANCODE_W]){
		y_vec -= 1;
	}
	if(current_key_states[SDL_SCANCODE_A]){
		x_vec -= 1;
	}
	if(current_key_states[SDL_SCANCODE_S]){
		y_vec += 1;
	}
	if(current_key_states[SDL_SCANCODE_D]){
		x_vec += 1;	
	}

	x += x_vec * speed * timedelta;
	y += y_vec * speed * timedelta;
}

void Player::check_if_colliding(Actor* target){

}