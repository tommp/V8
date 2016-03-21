#include "mob.h"

Mob::Mob(Resource_manager& manager, const std::string& model_name, const std::string& mob_name){
	name = mob_name;
	if ( !(model = manager.load_model(model_name) ) ){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Mob constructor failed to load model: " << model_name << std::endl;
		errorlogger("FATAL ERROR: Mob constructor failed to load model: ", model_name.c_str());
		exit(EXIT_FAILURE);
	}

	velocity = {0.0f, 0.0f, 0.0f};
	
}

Mob::~Mob(){
	
}

bool Mob::update_position(float timedelta){
	btTransform transform;
    motion_state->getWorldTransform(transform);

    position[0] = transform.getOrigin().getX();
    position[1] = transform.getOrigin().getY();
    position[2] = transform.getOrigin().getZ();

	if ( SDL_GetTicks() > last_move + move_duration && SDL_GetTicks() < last_move + (2*move_duration)) {
		velocity[0] = 0;
		velocity[1] = 0;
	}
	else if(SDL_GetTicks() > last_move + (2*move_duration)){
		velocity[0] = rand()%3 - 1;
		velocity[2] = rand()%3 - 1;
		last_move = SDL_GetTicks();
		move_duration = rand()%1000;
		if(glm::length(velocity)){
			velocity = glm::normalize(velocity);
			direction = velocity;
			velocity *= speed;
		}
	}

	last_pos = position;

	collision_body->setLinearVelocity(btVector3(velocity.x,velocity.y,velocity.z));
	return true;
}

bool Mob::touch_object(Object& object){
	return true;
}

bool Mob::add_contexts_to_renderer(Renderer& renderer)const{
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