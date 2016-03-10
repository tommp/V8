#include "prop.h"

Prop::Prop(Resource_manager& manager){
	std::string model_name = "engine_base_floor";
	state = {"prop"};
	if ( !(model = manager.load_model(model_name) ) ){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Prop constructor failed to load model: " << model_name << std::endl;
		errorlogger("ERROR: Prop constructor failed to load model: ", model_name.c_str());
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
	model->render_model(renderer, position, size, direction);
}

bool Prop::update_position(float timedelta){
	return true;
}

bool Prop::touch_object(Object& object){
	return true;
}
