#include "prop.h"

Prop::Prop(){
	std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: No default prop constructor!" << std::endl;
	errorlogger("FATAL ERROR: No default prop constructor!");
	exit(EXIT_FAILURE);
}

Prop::Prop(Resource_manager& manager, const std::string& model_name){
	glm::vec4 color;
	color.x = (rand()%100) /100.0f;
	color.y = (rand()%100) /100.0f;
	color.z = (rand()%100) /100.0f;
	color.w = 0.8;

	this->color = color;

	if (!(model = manager.load_model(model_name, color, 1.0))){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Prop constructor failed to load model: " << model_name << std::endl;
		errorlogger("FATAL ERROR: Prop constructor failed to load model: ", model_name.c_str());
		exit(EXIT_FAILURE);
	}

	this->color_coeff = glm::vec4(1.0);

	model->bind_context(model_matrix, context_name, color_coeff);

	position = {0, 0, 0};
	direction = {0.0f, 0.0f, -1.0f};
	scale = {10.0f, 10.0f, 10.0f};

	prev_position = {10.0, 0.0, 0.0};

	/* Physics */
	GLfloat mass = 10;
	btQuaternion rotation = {0.0, 0.0, 0.0, 1.0};
	generate_collision_volume(BOX, scale);
	generate_collision_body(mass, rotation, position);
}

Prop::Prop(Resource_manager& manager, 
		const std::string& model_name,
		const glm::vec3& position,
		const glm::vec3& scale,
		const glm::vec3& direction,
		const glm::vec4& color,
		const glm::vec4& color_coeff,
		GLfloat mass,
		Collision_shape shape,
		const glm::vec3& collision_scale){

	this->color = color;
	this->color_coeff = color_coeff;

	if (!(model = manager.load_model(model_name, color, 1.0))){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Prop constructor failed to load model: " << model_name << std::endl;
		errorlogger("FATAL ERROR: Prop constructor failed to load model: ", model_name.c_str());
		exit(EXIT_FAILURE);
	}

	model->bind_context(model_matrix, context_name, this->color_coeff);

	this->position = position;
	this->direction = glm::normalize(direction);
	this->scale = scale;

	prev_position = position + glm::vec3(10.0, 0.0, 0.0);

	/* Physics */
	btQuaternion rotation = {0.0, 0.0, 0.0, 1.0};
	generate_collision_volume(shape, collision_scale);
	generate_collision_body(mass, rotation, position);
}




Prop::~Prop(){
	if (!model->unbind_context(context_name)){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to unbind model context: " << context_name << std::endl;
		errorlogger("FATAL ERROR: Failed to unbind model context: ", context_name.c_str());
		exit(EXIT_FAILURE);
	}
}

bool Prop::update_position(GLfloat timedelta){
	(void(timedelta));

	if (!update_matrices()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to update matrices for prop!" << std::endl;
		errorlogger("ERROR: Failed to update matrices for prop!");
		return false;
	}

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
	update_transform();
	update_model_matrix();
	fill_glm_matrix(model_matrix);
	model_matrix = glm::scale(model_matrix, scale);

	return true;
}