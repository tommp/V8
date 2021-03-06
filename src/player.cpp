#include "player.h"

Player::Player(){
	std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: No default player constructor!" << std::endl;
	errorlogger("FATAL ERROR: No default player constructor!");
	exit(EXIT_FAILURE);
}

Player::Player(Resource_manager& init_manager, const std::string& model_name){
	glm::vec4 color = glm::vec4(0.5, 0.0, 1.0, 0.8);
	if (!(model = init_manager.load_model(model_name, color, 1.0))){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Player constructor failed to load model: " << model_name << std::endl;
		errorlogger("ERROR: Player constructor failed to load model: ", model_name.c_str());
	}

	this->color_coeff = glm::vec4(1.0);

	model->bind_context(model_matrix, context_name, color_coeff);

	manager = &init_manager;
	speed = 1000.0f;
	velocity = {0.0f, 0.0f, 0.0f};

	position = {10.0f, 10.0f, 10.0f};
	scale = {1.0f, 1.0f, 1.0f};
	direction = {0.0f, 0.0f, -1.0f};

	glm::vec3 lightcolor = {1.0f, 1.0f, 1.0f};
	glm::vec3 lightcomponents = {0.0f, 1.0f, 1.0f};

	prev_position = position * 2.0f;
	prev_scale = scale;
	prev_direction = direction;

	mass = 20.0f;
	btQuaternion rotation = {0.0, 1.0, 0.0, 0.0};
	generate_collision_volume(SPHERE, scale);
	generate_collision_body(mass, rotation, position);
	collision_body->setActivationState(DISABLE_DEACTIVATION);
}

Player::~Player(){
	if (!model->unbind_context(context_name)){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to unbind model context: " << context_name << std::endl;
		errorlogger("FATAL ERROR: Failed to unbind model context: ", context_name.c_str());
		exit(EXIT_FAILURE);
	}
}

bool Player::update_position(GLfloat timedelta, const glm::vec3& dir, const glm::vec3& right){
	(void(timedelta));
	velocity = {0.0f, 0.0f, 0.0f};
	const Uint8* current_key_states = SDL_GetKeyboardState(NULL);
	if(current_key_states[manager->get_button_map_key("player", UP)]){
		velocity.x -= dir.x;
		velocity.z -= dir.z;
	}
	if(current_key_states[manager->get_button_map_key("player", LEFT)]){
		velocity.x -= right.x;
		velocity.z -= right.z;
	}
	if(current_key_states[manager->get_button_map_key("player", DOWN)]){
		velocity.x += dir.x;
		velocity.z += dir.z;
	}
	if(current_key_states[manager->get_button_map_key("player", RIGHT)]){
		velocity.x += right.x;
		velocity.z += right.z;

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
		velocity *= 40;
	}

	
	update_matrices();
	collision_body->setLinearVelocity(btVector3(velocity.x,velocity.y,velocity.z));
	return true;
}

bool Player::add_contexts_to_renderer(Renderer& renderer)const{
	if (!model->add_contexts_to_renderer(renderer)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add model contexts to renderer!" << std::endl;
		errorlogger("ERROR: Failed to add model contexts to renderer!");
		return false;
	}

	return true;
}

bool Player::update_matrices(){
	update_transform();
	update_model_matrix();
	fill_glm_matrix(model_matrix);
	model_matrix = glm::scale(model_matrix, scale);

	return true;
}