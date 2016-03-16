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

	rendering_context->active = true;
	rendering_context->init_direction = {0.0f, 1.0f, -1.0f};
	add_bases_to_context();
}

Prop::~Prop(){
	
}

bool Prop::update_position(float timedelta){
	return true;
}

bool Prop::update_context() {
	return true;
}

bool Prop::touch_object(Object& object){
	return true;
}

bool Prop::add_bases_to_context() {
	if (!model->add_bases_to_context(*rendering_context)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to add model bases to rendering context!" << std::endl;
		errorlogger("ERROR: Failed to add model bases to rendering context!");
		return false;
	}

	return true;
}