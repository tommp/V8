#include "player.h"

Player::Player(Resource_manager& init_manager, const std::string& model_name){
	if (!(model = init_manager.load_model(model_name))){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Player constructor failed to load model: " << model_name << std::endl;
		errorlogger("ERROR: Player constructor failed to load model: ", model_name.c_str());
	}

	manager = &init_manager;
	speed = 400.0f;
	velocity = {0.0f, 0.0f, 0.0f};

	position = {0.0f, 0.0f, 0.0f};
	scale = {20.0f, 20.0f, 20.0f};
	direction = {0.0f, 0.0f, -1.0f};

	prev_position = position * 2.0f;
	prev_scale = scale;
	prev_direction = direction;

	mass = 100.0f;
	fall_inertia = {0.0, 0.0, 0.0};
	collision_shape = new btSphereShape(20);
	collision_shape->calculateLocalInertia(mass, fall_inertia);
	motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), 
														btVector3(0, 0, 0)));
	
	btRigidBody::btRigidBodyConstructionInfo collision_body_CI(mass, 
															motion_state, 
															collision_shape, 
															btVector3(0, 0, 0));
	collision_body = new btRigidBody(collision_body_CI);
}

bool Player::update_model_matrix() {
	GLboolean should_update_model = false;
	if (position != prev_position) {
		should_update_model = true;
	}
	else if (direction != prev_direction) {
		should_update_model = true;
	}

	else if (scale != prev_scale) {
		should_update_model = true;
	}

	if (should_update_model) {
		model->update_matrices(position, scale, direction);
		prev_position = position;
		prev_scale = scale;
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

	update_model_matrix();
	collision_body->setLinearVelocity(btVector3(velocity.x,velocity.y,velocity.z));
	return true;
}

bool Player::touch_object(Object& object){
	return true;
}

bool Player::add_contexts_to_renderer(Renderer& renderer)const{
	if (!model->add_mesh_contexts_to_renderer(renderer)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add model mesh contexts to renderer!" << std::endl;
		errorlogger("ERROR: Failed to add model mesh contexts to renderer!");
		return false;
	}
	if (!model->add_light_contexts_to_renderer(renderer)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add model light contexts to renderer!" << std::endl;
		errorlogger("ERROR: Failed to add model light contexts to renderer!");
		return false;
	}

	return true;
}