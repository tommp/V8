#include "prop.h"

Prop::Prop(Resource_manager& manager, const std::string& model_name){
	if (!(model = manager.load_model(model_name))){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Prop constructor failed to load model: " << model_name << std::endl;
		errorlogger("ERROR: Prop constructor failed to load model: ", model_name.c_str());
	}

	position[0] = 2000.0f;
	position[1] = -10.0f;
	position[2] = 0.0f;

	direction = {0.0f, 0.0f, -1.0f};

	scale = {2000.0f, 10.0f, 2000.0f};

	btVector3 box_size = {scale.x, scale.y, scale.z};

	/* Physics */
	mass = 0;
	fall_inertia = {0, 0, 0};
	collision_shape = new btBoxShape(box_size);
	collision_shape->calculateLocalInertia(mass, fall_inertia);
	motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), 
														btVector3(0, position.y, position.z)));
	btRigidBody::btRigidBodyConstructionInfo collision_body_CI(mass, motion_state, collision_shape, 
															btVector3(0, 0, 0));
	collision_body = new btRigidBody(collision_body_CI);

	update_model_matrix();
}

Prop::~Prop(){
	
}

bool Prop::update_position(GLfloat timedelta){
	return true;
}

bool Prop::update_model_matrix() {
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

bool Prop::touch_object(Object& object){
	return true;
}

bool Prop::add_contexts_to_renderer(Renderer& renderer)const{
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