#include "player.h"

Player::Player(){
	std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: No default mob constructor!" << std::endl;
	errorlogger("FATAL ERROR: No default mob constructor!");
	exit(EXIT_FAILURE);
}

Player::Player(Resource_manager& init_manager, const std::string& model_name){
	glm::vec4 color;
	color.x = (rand()%100) /100.0f ;
	color.y = (rand()%100) /10.0f;
	color.z = (rand()%100) /100.0f;
	color.w = 0.0;
	if (!(model = init_manager.load_model(model_name, color))){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Player constructor failed to load model: " << model_name << std::endl;
		errorlogger("ERROR: Player constructor failed to load model: ", model_name.c_str());
	}

	model->bind_context(model_matrix, normal_model_matrix, context_name);

	manager = &init_manager;
	speed = 400.0f;
	velocity = {0.0f, 0.0f, 0.0f};

	position = {100.0f, 100.0f, 100.0f};
	scale = {20.0f, 20.0f, 20.0f};
	direction = {0.0f, 0.0f, -1.0f};

	glm::vec3 lightcolor = {1.0f, 1.0f, 1.0f};
	glm::vec3 lightcomponents = {0.0f, 1.0f, 1.0f};

	flashlight = std::make_shared<Spot_light>(600, 
				position,
				direction,
				lightcolor, 
				lightcomponents,
				30,
				60,
				1.0);

	flashlight->set_intensity(0.0);

	prev_position = position * 2.0f;
	prev_scale = scale;
	prev_direction = direction;

	mass = 100.0f;
	btQuaternion rotation = {0.0, 1.0, 0.0, 0.0};
	generate_collision_volume(model_name, SPHERE, scale);
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

bool Player::update_position(GLfloat timedelta, const glm::mat4& view_matrix){

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

	update_matrices(view_matrix);
	flashlight->set_direction(direction);
	flashlight->set_position(get_position());
	flashlight->calculate_light_uniforms();
	collision_body->setLinearVelocity(btVector3(velocity.x,velocity.y,velocity.z));
	return true;
}

bool Player::touch_object(Object& object){
	return true;
}

bool Player::add_contexts_to_renderer(Renderer& renderer)const{
	if (!model->add_contexts_to_renderer(renderer)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add model contexts to renderer!" << std::endl;
		errorlogger("ERROR: Failed to add model contexts to renderer!");
		return false;
	}

	flashlight->add_context(renderer);

	return true;
}

bool Player::update_matrices(const glm::mat4& view_matrix){
	update_transform();
	update_model_matrix();
	fill_glm_matrix(model_matrix);
	model_matrix = glm::scale(model_matrix, scale);
	normal_model_matrix = glm::inverseTranspose(glm::mat3(view_matrix * model_matrix));
	

	return true;
}