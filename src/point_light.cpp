#include "point_light.h"

Point_light::Point_light(GLfloat radius, 
						const glm::vec3& pos, 
						const glm::vec3& color, 
						const glm::vec3& color_components) {
	base_light_context->shader_type = LIGHT_POINT;

	this->radius = radius;
	this->color = color;
	this->color_components = color_components;
	this->position = pos;

	if (!calculate_light_uniforms()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to calculate light uniforms for point light!" << std::endl;
		errorlogger("FATAL ERROR: Failed to calculate light uniforms for point light!");
		exit(EXIT_FAILURE);
	}

	if (!bind_lambda_expression()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to bind lambda expression for point light!" << std::endl;
		errorlogger("FATAL ERROR: Failed to bind lambda expression for point light!");
		exit(EXIT_FAILURE);
	}
}

Point_light::Point_light(){
	base_light_context->shader_type = LIGHT_POINT;
	radius = rand() % 500;
	randomize_position(glm::i16vec3(1000, 1, 1000), glm::i16vec3(0, -100, 0));
	randomize_color(5);
	color_components = {0.0f, 1.0f, 1.0f};

	if (!calculate_light_uniforms()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to calculate light uniforms for point light!" << std::endl;
		errorlogger("FATAL ERROR: Failed to calculate light uniforms for point light!");
		exit(EXIT_FAILURE);
	}

	if (!bind_lambda_expression()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to bind lambda expression for point light!" << std::endl;
		errorlogger("FATAL ERROR: Failed to bind lambda expression for point light!");
		exit(EXIT_FAILURE);
	}
}

bool Point_light::bind_lambda_expression()const{
	base_light_context->setup_base_uniforms = [&](const Shader_ptr& shader) {
		if (!shader) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Null shader passed when rendering point light!" << std::endl;
			errorlogger("ERROR: Null shader passed when rendering point light!");
			return false;
		}

		glUniformMatrix4fv(shader->load_uniform_location("model"),
						 1, 
						 GL_FALSE, 
						 glm::value_ptr(quad_model_matrix));
		
		if(check_ogl_error()){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to set model matrix for point light!" << std::endl;
			errorlogger("ERROR: Failed to set model matrix for point light!");
			return false;
		}

		glUniform3fv(shader->load_uniform_location("light.position"), 1, (float*)&(position));
		glUniform1f(shader->load_uniform_location("light.radius"), radius);

		glUniform3fv(shader->load_uniform_location("light.color"), 1, (float*)&(color));
		glUniform3fv(shader->load_uniform_location("light.color_components"), 1, (float*)&(color_components));
		if(check_ogl_error()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind point light uniforms!" << std::endl;
			errorlogger("ERROR: Failed to bind point light uniforms!");
			return false;
		}

		return true;
	};

	return true;
}

bool Point_light::calculate_light_uniforms(){

	scale = {radius * 2, radius * 2, radius * 2};

	quad_model_matrix = glm::mat4();
	quad_model_matrix = glm::translate(quad_model_matrix, position);  
	quad_model_matrix = glm::scale(quad_model_matrix, scale); 

	return true;
}