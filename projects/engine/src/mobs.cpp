#include "mobs.h"

Cube::Cube(Resource_manager& manager){
	std::string model_name = "wiggle";
	state = {"wiggle"};
	if ( !(model = manager.load_model(model_name) ) ){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Cube constructor failed to load model: " << model_name << std::endl;
		errorlogger("FATAL ERROR: Cube constructor failed to load model: ", model_name.c_str());
		exit(EXIT_FAILURE);
	}
	speed = 30.0f;
	position[0] = rand() % 4001;
	position[1] = 0;
	position[2] = rand() % 4001;

	velocity = {0.0f, 0.0f, 0.0f};

	last_move = SDL_GetTicks();
	move_duration = rand()%1000;
	size = {20.0f, 20.0f, 20.0f};

	/* Physics */
	mass = 100;
	fall_inertia = {0, 0, 0};
	collision_shape = new btSphereShape(30);
	collision_shape->calculateLocalInertia(mass, fall_inertia);
	motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), 
														btVector3(position.x, position.y, position.z)));
	btRigidBody::btRigidBodyConstructionInfo collision_body_CI(mass, motion_state, collision_shape, 
															btVector3(0, 0, 0));
	collision_body = new btRigidBody(collision_body_CI);

	update_model_matrix();
	rendering_context->active = true;
	rendering_context->init_direction = {0.0f, 1.0f, -1.0f};
	add_bases_to_context();
}

Cube::~Cube(){
	
}

bool Cube::update_context() {
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

bool Cube::update_position(float timedelta){
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

bool Cube::touch_object(Object& object){
	return true;
}

bool Cube::add_bases_to_context() {
	if (!model->add_bases_to_context(*rendering_context)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to add model bases to rendering context!" << std::endl;
		errorlogger("ERROR: Failed to add model bases to rendering context!");
		return false;
	}

	return true;
}

void Cube::update_model_matrix() {
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