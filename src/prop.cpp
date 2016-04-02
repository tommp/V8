#include "prop.h"

Prop::Prop(Resource_manager& manager, const std::string& model_name){
	if (!(model = manager.load_model(model_name))){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Prop constructor failed to load model: " << model_name << std::endl;
		errorlogger("ERROR: Prop constructor failed to load model: ", model_name.c_str());
	}

	model->bind_context(model_matrix, normal_model_matrix, context_name);

	position = {0, 0, 0};
	direction = {0.0f, 0.0f, -1.0f};
	scale = {200.0f, 10.0f, 200.0f};

	prev_position = position * 2.0f;

	/* Physics */
	GLfloat mass = 0;
	glm::vec3 inertia = {0.0, 0.0, 0.0};
	btQuaternion rotation = {0.0, 0.0, 0.0, 1.0};
	generate_collision_volume(model_name, BOX, scale);
	generate_collision_body(mass, inertia, rotation, position);

	update_matrices();
}

Prop::~Prop(){
	
}

bool Prop::update_position(GLfloat timedelta){
	return true;
}

bool Prop::touch_object(Object& object){
	return true;
}

bool Prop::add_contexts_to_renderer(Renderer& renderer)const{
	if (!model->add_contexts_to_renderer(renderer)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add model contexts to renderer!" << std::endl;
		errorlogger("ERROR: Failed to add model contexts to renderer!");
		return false;
	}

	return true;
}

bool Prop::update_matrices(){
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
		//GLfloat dot = glm::dot(direction, model->get_init_direction());
		//GLfloat det =  model->get_init_direction().x*direction.z - model->get_init_direction().z*direction.x;
		//GLfloat rotation = -1 * glm::atan(det, dot);

	    //model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z)); 
	    //model_matrix = glm::rotate(model_matrix, rotation, glm::vec3(0.0f, 1.0f, 0.0f)); 
	    //model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.5f * size.z));

	    model_matrix = glm::scale(model_matrix, glm::vec3(scale));

	    normal_model_matrix = glm::inverseTranspose(glm::mat3(model_matrix));

		prev_position = position;
		prev_scale = scale;
		prev_direction = direction;
	}
	return true;
}