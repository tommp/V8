#include "prop.h"

Prop::Prop(Resource_manager& manager){
	std::string anim_set = "floor";
	state = {"prop"};
	if ( !(animations = manager.load_animation_set(anim_set) ) ){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Slime_blob constructor failed to load animation set: " << anim_set << std::endl;
		errorlogger("ERROR: Slime_blobconstructor failed to load animation set: ", anim_set.c_str());
	}

	position[0] = 10000.0f;
	position[1] = -10.0f;
	position[2] = 10000.0f;

	size = {20000.0f, 10.0f, 20000.0f};

	btVector3 box_size = {size.x*1000, size.y, size.z*1000};
	/* Physics */
	mass = 0;
	fall_inertia = {0, 0, 0};
	collision_shape = new btBoxShape(box_size);
	collision_shape->calculateLocalInertia(mass, fall_inertia);
	motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), 
														btVector3(position.x, position.y, position.z)));
	btRigidBody::btRigidBodyConstructionInfo collision_body_CI(mass, motion_state, collision_shape, 
															btVector3(0, 0, 0));
	collision_body = new btRigidBody(collision_body_CI);
}

Prop::~Prop(){
	
}

void Prop::render_frame(const Renderer& renderer)const{
	animations->render_current(renderer, state, position, size, direction);
}

bool Prop::update_position(float timedelta){
	return true;
}

bool Prop::touch_character(Character& character){
	return true;
}
