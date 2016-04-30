#include "entity.h"

bool Entity::init_entity(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& dir){
	this->position = pos;
	this->scale = scale;
	this->direction = dir;
	this->init_direction = dir;

	return true;
}

bool Entity::init_model_context(const std::string& model_name){
	if (!model_context.load_model(*manager, model_name)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to load model for entity: " << name << std::endl;
		errorlogger("ERROR: Failed to load model for entity: ", name.c_str());
		return false;
	}

	has_model = true;
	return true;
}

bool Entity::init_model_context(const std::string& model_name, const glm::vec4& color, GLfloat gloss){
	if (!model_context.load_model(*manager, model_name, color,  gloss)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to load model with name: " << model_name << ", for entity: " << name << std::endl;
		errorlogger("ERROR: Failed to load model for entity: ", name.c_str());
		return false;
	}

	has_model = true;
	return true;
}

bool Entity::init_physics_context(Collision_shape shape, GLfloat mass, GLboolean allow_deactivation){
	physics_context = Object();
	btQuaternion rotation = {0.0, 1.0, 0.0, 0.0};
	if (has_model){
		if (!physics_context.generate_collision_volume(shape, model_context.model->get_name(), scale) || !physics_context.generate_collision_body(mass, rotation, position)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to generate collision data for entity: " << name << std::endl;
			errorlogger("ERROR: Failed to generate collision data for entity: ", name.c_str());
			return false;
		}
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: No model loaded for entity: " << name << ", if you want the collision volumes to sync properly, load the model first!" << std::endl;
		if (!physics_context.generate_collision_volume(shape, scale) || !physics_context.generate_collision_body(mass, rotation, position)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to generate collision data for entity: " << name << std::endl;
			errorlogger("ERROR: Failed to generate collision data for entity: ", name.c_str());
			return false;
		}
	}

	if (!allow_deactivation) {
		physics_context.disable_deactivation();
	}
	else{
		physics_context.enable_deactivation();
	}
	

	has_physics = true;
	return true;
}

bool Entity::update_position(){
	
	
	return true;
}

bool Entity::update_physics_context(){
	if (!has_physics){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: No physics context to update for entity: " << name << std::endl;
		errorlogger("ERROR: No physics context to update for entity: ", name.c_str());
		return false;
	}

	if (!physics_context.update_transform() && physics_context.update_model_matrix()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to update physics context for entity: " << name << std::endl;
		errorlogger("ERROR: Failed to update physics context for entity: ", name.c_str());
		return false;
	}
	
	return true;
}

bool Entity::update_model_context(const glm::mat4& view_matrix){
	if (!has_model){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: No model context to update for entity: " << name << std::endl;
		errorlogger("ERROR: No model context to update for entity: ", name.c_str());
		return false;
	}

	if (has_physics) {
		if (!physics_context.fill_glm_matrix(model_context.model_view_matrix)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to fill model view matrix for entity: " << name << std::endl;
			errorlogger("ERROR: Failed to fill model view matrix for entity: ", name.c_str());
			return false;
		}
	}else{
		model_context.model_view_matrix = glm::mat4();
		model_context.model_view_matrix = glm::translate(model_context.model_view_matrix, position);
		/* TODO:: Rotate based on actor directions */
	}

	model_context.model_view_matrix = glm::scale(model_context.model_view_matrix, scale);
	model_context.model_view_matrix = view_matrix * model_context.model_view_matrix;

	return true;
}

bool Entity::synchronize_contexts(const glm::mat4& view_matrix){
	if (!update_physics_context()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to update physics context for entity: " << name << std::endl;
		errorlogger("ERROR: Failed to update physics context for entity: ", name.c_str());
		return false;
	}

	if (!update_model_context(view_matrix)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to update model context for entity: " << name << std::endl;
		errorlogger("ERROR: Failed to update model context for entity: ", name.c_str());
		return false;
	}

	return true;
}