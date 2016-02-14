#include "player.h"

Player::Player(Resource_manager& init_manager){
	animations = std::make_shared<Animation_set>();
	animations->load_from_file(init_manager, "player");
	state = "player_test";
	manager = &init_manager;
	speed = 800.0f;
	position = {100.0f, 0.0f, 100.0f};
	size = {1.0f, 1.0f, 1.0f};
	direction = {0.0f, 0.0f, -1.0f};
	velocity = {0.0f, 0.0f, 0.0f};
}

void Player::render_frame(const Renderer& renderer)const{
	animations->render_current(renderer, state, position, size, direction);
}

void Player::update_position(GLfloat timedelta){
	velocity = {0.0f, 0.0f, 0.0f};
	const Uint8* current_key_states = SDL_GetKeyboardState(NULL);
	if(current_key_states[manager->get_button_map_key("player", UP)]){
		velocity[2] -= 1;
	}
	if(current_key_states[manager->get_button_map_key("player", LEFT)]){
		velocity[0] -= 1;
	}
	if(current_key_states[manager->get_button_map_key("player", DOWN)]){
		velocity[2] += 1;
	}
	if(current_key_states[manager->get_button_map_key("player", RIGHT)]){
		velocity[0] += 1;

	}
	if(glm::length(velocity)) {
		velocity = glm::normalize(velocity);
		direction = velocity;
		velocity *= speed;
	}

	position[0] += velocity[0] * timedelta;
	//position[1] += velocity[1] * speed * timedelta;
	position[2] += velocity[2] * timedelta;
}

void Player::touch_character(Character& character){

}