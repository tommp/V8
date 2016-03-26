#include "mob.h"

Mob::Mob(Resource_manager& manager, const std::string& model_name, const std::string& mob_name){
	name = mob_name;
	if ( !(model = manager.load_model(model_name) ) ){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Mob constructor failed to load model: " << model_name << std::endl;
		errorlogger("FATAL ERROR: Mob constructor failed to load model: ", model_name.c_str());
		exit(EXIT_FAILURE);
	}

	model->bind_context(model_matrix, normal_model_matrix, context_name);

	scale = {50.0f, 50.0f, 50.0f};


	velocity = {0.0f, 0.0f, 0.0f};
	last_move = 0;
	move_duration = 4000;
	speed = 400;

	mass = 100.0f;
	fall_inertia = {0.0, 0.0, 0.0};
	collision_shape = new btSphereShape(50);
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
	if (!model->unbind_context(context_name)){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to unbind model context: " << context_name << std::endl;
		errorlogger("FATAL ERROR: Failed to unbind model context: ", context_name.c_str());
		exit(EXIT_FAILURE);
	}
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

	if (!update_matrices()) {
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
	if (!model->add_contexts_to_renderer(renderer)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add model contexts to renderer!" << std::endl;
		errorlogger("ERROR: Failed to add model contexts to renderer!");
		return false;
	}

	return true;
}

bool Mob::update_matrices(){
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
		model_matrix = glm::mat4();
		model_matrix = glm::translate(model_matrix, position);  

		/* TODO:: 3D rotation */
		GLfloat dot = glm::dot(direction, model->get_init_direction());
		GLfloat det =  model->get_init_direction().x*direction.z - model->get_init_direction().z*direction.x;
		GLfloat rotation = -1 * glm::atan(det, dot);

	    //model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z)); 
	    model_matrix = glm::rotate(model_matrix, rotation, glm::vec3(0.0f, 1.0f, 0.0f)); 
	    //model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.5f * size.z));

	    model_matrix = glm::scale(model_matrix, glm::vec3(scale));

	    normal_model_matrix = glm::inverseTranspose(glm::mat3(model_matrix));

		prev_position = position;
		prev_scale = scale;
		prev_direction = direction;
	}
	return true;
}