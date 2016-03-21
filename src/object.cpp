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

bool Object::delete_collision_data() {
	delete motion_state;
	delete collision_shape;
	delete collision_body;

	return true;
}

btRigidBody* Object::get_collision_body()const{
	return collision_body;
}