#include "player.h"

Player::Player(Resource_manager& init_manager){
	std::string model_name = "sphere_colored";
	if ( !(model = init_manager.load_model(model_name) ) ){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Player constructor failed to load model: " << model_name << std::endl;
		errorlogger("ERROR: Player constructor failed to load model: ", model_name.c_str());
	}

	manager = &init_manager;
	speed = 400.0f;
	position = {0.0f, 0.0f, 0.0f};
	size = {20.0f, 20.0f, 20.0f};
	direction = {0.0f, 0.0f, -1.0f};
	velocity = {0.0f, 0.0f, 0.0f};
}

void Player::update_model_matrix() {
	rendering_context->model_matrix = glm::mat4();
	rendering_context->model_matrix = glm::translate(rendering_context->model_matrix, position);  

	/* TODO:: 3D rotation */
	GLfloat dot = glm::dot(direction, rendering_context->init_direction);
	GLfloat det =  rendering_context->init_direction.x*direction.z - rendering_context->init_direction.z*direction.x;
	GLfloat rotation = -1 * glm::atan(det, dot);

    //model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z)); 
    rendering_context->model_matrix = glm::rotate(rendering_context->model_matrix, rotation, glm::vec3(0.0f, 1.0f, 0.0f)); 
    //model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.5f * size.z));

    rendering_context->model_matrix = glm::scale(rendering_context->model_matrix, glm::vec3(size));
}

bool Player::update_context() {
	GLboolean should_update_model = false;
	if (position != prev_position) {
		should_update_model = true;
	}
	else if (direction != prev_direction) {
		should_update_model = true;
	}

	else if (size != prev_size) {
		should_update_model = true;
	}

	if (should_update_model) {
		update_model_matrix();
		prev_position = position;
		prev_size = size;
		prev_direction = direction;
	}
	return true;
}

bool Player::update_position(GLfloat timedelta){

    btVector3 phys_position = collision_body->getCenterOfMassPosition();

    position[0] = phys_position.getX();
    position[1] = phys_position.getY();
    position[2] = phys_position.getZ();

    //std::cout <<"player: " << position[0] << ":" << position[1] << ":" << position[2] << std::endl;

	velocity = {0.0f, 0.0f, 0.0f};
	const Uint8* current_key_states = SDL_GetKeyboardState(NULL);
	if(current_key_states[manager->get_button_map_key("player", UP)]){
		velocity.z -= 1;
	}
	if(current_key_states[manager->get_button_map_key("player", LEFT)]){
		velocity.x -= 1;
	}
	if(current_key_states[manager->get_button_map_key("player", DOWN)]){
		velocity.z += 1;
	}
	if(current_key_states[manager->get_button_map_key("player", RIGHT)]){
		velocity.x += 1;

	}
	if(current_key_states[manager->get_button_map_key("player", JUMP)]){
		velocity.y += 1;

	}
	if(current_key_states[manager->get_button_map_key("player", INTERACT)]){
		velocity.y -= 1;

	}
	if(glm::length(velocity)) {
		velocity = glm::normalize(velocity);
		direction = velocity;
		velocity *= speed;
	}

	collision_body->setLinearVelocity(btVector3(velocity.x,velocity.y,velocity.z));
	return true;
}

bool Player::touch_object(Object& object){
	return true;
}

bool Player::add_bases_to_context() {
	if (!model->add_bases_to_context(*rendering_context)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to add model bases to rendering context!" << std::endl;
		errorlogger("ERROR: Failed to add model bases to rendering context!");
		return false;
	}

	return true;
}