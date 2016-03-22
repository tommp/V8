#include "object.h"

Object::~Object() {
	if (!delete_collision_data()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to delete collision data!" << std::endl;
		errorlogger("FATAL ERROR: Failed to delete collision data!");
		exit(EXIT_FAILURE);
	}
}

Object::Object() {
	mass = 0;
	fall_inertia = {0.0, 0.0, 0.0};
	/* TODO::Properly set this */
	collision_shape = nullptr;
	motion_state = nullptr;
	collision_body = nullptr;
}

bool Object::delete_collision_data() {
	delete motion_state;
	delete collision_shape;
	delete collision_body;

	return true;
}

btRigidBody* Object::get_collision_body()const{
	return collision_body;
}