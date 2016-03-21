#include "mob.h"

Mob::Mob(Resource_manager& manager, const std::string& model_name, const std::string& mob_name){
	name = mob_name;
	if ( !(model = manager.load_model(model_name) ) ){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Mob constructor failed to load model: " << model_name << std::endl;
		errorlogger("FATAL ERROR: Mob constructor failed to load model: ", model_name.c_str());
		exit(EXIT_FAILURE);
	}

	scale = {50.0f, 50.0f, 50.0f};


	velocity = {0.0f, 0.0f, 0.0f};
	last_move = 0;
	move_duration = 4000;
	speed = 400;

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

Mob::~Mob(){
}

bool Mob::update_position(float timedelta){
	btTransform transform;
    motion_state->getWorldTransform(transform);

    position[0] = transform.getOrigin().getX();
    position[1] = transform.getOrigin().getY();
    position[2] = transform.getOrigin().getZ();

    //std::cout << "pos: " << position[0] << " : " << position[2] << std::endl;

	if ( SDL_GetTicks() > last_move + move_duration && SDL_GetTicks() < last_move + (2*move_duration)) {
		velocity[0] = 0;
		velocity[2] = 0;
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

	if (!update_model_matrix()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to update mob model matrix!" << std::endl;
		errorlogger("ERROR: Failed to update mob model matrix!");
		return false;
	}

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

bool Mob::update_model_matrix(){
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