#include "mobs.h"

Cube::Cube(Resource_manager& manager){
	std::string anim_set = "cube";
	state = {"cube_walk"};
	if ( !(animations = manager.load_animation_set(anim_set) ) ){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Slime_blob constructor failed to load animation set: " << anim_set << std::endl;
		errorlogger("ERROR: Slime_blobconstructor failed to load animation set: ", anim_set.c_str());
	}
	speed = 30.0f;
	position[0] = rand() % 4001;
	position[1] = 0;
	position[2] = rand() % 4001;

	velocity = {0.0f, 0.0f, 0.0f};

	last_move = SDL_GetTicks();
	move_duration = rand()%1000;
	size = {2.0f, 2.0f, 2.0f};

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
}

Cube::~Cube(){
	
}

void Cube::render_frame(const Renderer& renderer)const{
	animations->render_current(renderer, state, position, size, direction);
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

bool Cube::touch_character(Character& character){
	return true;
}
