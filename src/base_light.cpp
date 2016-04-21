#include "base_light.h"

Base_light::Base_light(){
	color = {0.0f, 0.0f, 0.0f};
	color_components = {0.0f, 0.0f, 0.0f};

	scale = {100.0f, 100.0f, 100.0f};

	render_shadows = 0;
	apply_SSAO = 0;

	quad_model_matrix = glm::mat4();
	quad_model_matrix = glm::translate(quad_model_matrix, position);  
	quad_model_matrix = glm::scale(quad_model_matrix, scale); 

	base_light_context = std::make_shared<Rendering_context_light>();
	base_light_context->shader_type = NO_SHADER;
}

bool Base_light::randomize_position(const glm::i16vec3& maxpos, const glm::i16vec3& offset){
	position.x = (rand() % (maxpos.x + 1)) - offset.x;
	position.y = (rand() % (maxpos.y + 1)) - offset.y;
	position.z = (rand() % (maxpos.z + 1)) - offset.z;

	return true;
}

bool Base_light::randomize_color(GLfloat intensity){
	color.x = (rand() % 1000) / 1000.0f;
	color.y = (rand() % 1000) / 1000.0f;
	color.z = (rand() % 1000) / 1000.0f;

	color *= intensity;

	return true;
}

bool Base_light::randomize_diffuse(){
	color_components.x = (rand() % 1000) / 1000.0f;
	return true;
}


bool Base_light::randomize_ambient(){
	color_components.y = (rand() % 1000) / 1000.0f;
	return true;
}

bool Base_light::randomize_specular(){
	color_components.z = (rand() % 1000) / 1000.0f;
	return true;
}

bool Base_light::add_context(Renderer& renderer)const{
	if (!renderer.add_context(base_light_context)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add light context to renderer!" << std::endl;
		errorlogger("ERROR: Failed to add light context to renderer!");
		return false;
	}

	return true;
}

bool Base_light::set_direction(const glm::vec3& dir){
	this->direction = dir;
	return true;
}

bool Base_light::set_position(const glm::vec3& pos) {
	this->position = pos;
	return true;
}