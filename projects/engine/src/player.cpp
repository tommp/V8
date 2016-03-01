#include "player.h"

Player::Player(Resource_manager& init_manager){
	animations = std::make_shared<Animation_set>();
	animations->load_from_file(init_manager, "player");
	state = "player_test";
	manager = &init_manager;
	speed = 400.0f;
	position = {0.0f, 0.0f, 0.0f};
	size = {2.0f, 2.0f, 2.0f};
	direction = {0.0f, 0.0f, -1.0f};
	velocity = {0.0f, 0.0f, 0.0f};

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

void Player::render_frame(const Renderer& renderer)const{
	animations->render_current(renderer, state, position, size, direction);
}

void Player::update_position(GLfloat timedelta){
	btTransform transform;
    motion_state->getWorldTransform(transform);

    position[0] = transform.getOrigin().getX();
    position[1] = transform.getOrigin().getY();
    position[2] = transform.getOrigin().getZ();

    //std::cout <<"player: " << position[0] << ":" << position[1] << ":" << position[2] << std::endl;

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
	if(current_key_states[manager->get_button_map_key("player", JUMP)]){
		velocity[1] += 1;

	}
	if(current_key_states[manager->get_button_map_key("player", INTERACT)]){
		velocity[1] -= 1;

	}
	if(glm::length(velocity)) {
		velocity = glm::normalize(velocity);
		direction = velocity;
		velocity *= speed;
	}

	collision_body->setLinearVelocity(btVector3(velocity.x,velocity.y,velocity.z));
}

void Player::touch_character(Character& character){

}