#include "model_context.h"

Model_context::Model_context(){
	complete = false;
}


bool Model_context::load_model(Resource_manager& init_manager, const std::string& model_name){
	this->model = init_manager.load_model(model_name);
	if (this->model == nullptr){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Resource manager failed to load model: " << model_name << std::endl;
		errorlogger("ERROR: Resource manager failed to load model: ", model_name.c_str());
		return false;
	}

	if (!this->model->bind_context(model_view_matrix, model_context_name)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind context for model: " << model_name << std::endl;
		errorlogger("ERROR: Failed to bind context for model: ", model_name.c_str());
		return false;
	}

	complete = true;

	return true;
} 

bool Model_context::load_model(Resource_manager& init_manager, 
						const std::string& model_name, 
						const glm::vec4& color, 
						GLfloat gloss){
	this->model = init_manager.load_model(model_name, color, gloss);
	if (this->model == nullptr){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Resource manager failed to load model: " << model_name << std::endl;
		errorlogger("ERROR: Resource manager failed to load model: ", model_name.c_str());
		return false;
	}

	if (!this->model->bind_context(model_view_matrix, model_context_name)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind context for model: " << model_name << std::endl;
		errorlogger("ERROR: Failed to bind context for model: ", model_name.c_str());
		return false;
	}

	complete = true;

	return true;
}

