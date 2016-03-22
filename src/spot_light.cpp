#include "spot_light.h"

Spot_light::Spot_light(GLfloat radius, 
						const glm::vec3& pos,
						const glm::vec3& dir,
						const glm::vec3& color, 
						const glm::vec3& color_components,
						const GLfloat cutoff,
						const GLfloat outer_cutoff) {
	base_light_context->shader_type = LIGHT_SPOT;

	this->radius = radius;
	this->position = pos;
	this->direction = dir;
	this->color = color;
	this->color_components = color_components;
	this->cutoff = cutoff;
	this->outer_cutoff = outer_cutoff;

	if (!calculate_light_uniforms()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to calculate light uniforms for spot light!" << std::endl;
		errorlogger("FATAL ERROR: Failed to calculate light uniforms for spot light!");
		exit(EXIT_FAILURE);
	}

	if (!bind_lambda_expression()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to bind lambda expression for spot light!" << std::endl;
		errorlogger("FATAL ERROR: Failed to bind lambda expression for spot light!");
		exit(EXIT_FAILURE);
	}
}

Spot_light::Spot_light(){
	base_light_context->shader_type = LIGHT_SPOT;
	direction = {0.0f, -1.0f, 0.0f};

	direction.x = ((rand() % 1000) / 500) - 1;
	direction.z = ((rand() % 1000) / 500) - 1;

	randomize_position(glm::i16vec3(4000, 0, 4000), glm::i16vec3(2000, -200, 2000));
	randomize_color(5);
	color_components = {0.0f, 1.0f, 1.0f};

	radius = rand() % 400;

	cutoff = (rand() % 30) + 15;
	outer_cutoff = cutoff + (rand() % 10) + 5;

	if (!calculate_light_uniforms()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to calculate light uniforms for spot light!" << std::endl;
		errorlogger("FATAL ERROR: Failed to calculate light uniforms for spot light!");
		exit(EXIT_FAILURE);
	}

	if (!bind_lambda_expression()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to bind lambda expression for spot light!" << std::endl;
		errorlogger("FATAL ERROR: Failed to bind lambda expression for spot light!");
		exit(EXIT_FAILURE);
	}
}


bool Spot_light::bind_lambda_expression()const{
	base_light_context->setup_base_uniforms = [&](const Shader_ptr& shader) {
		if (!shader) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: ERROR: Null shader passed when rendering spot light!" << std::endl;
			errorlogger("ERROR: Null shader passed when rendering spot light!");
			return false;
		}

		glUniformMatrix4fv(shader->load_uniform_location("model"),
						 1, 
						 GL_FALSE, 
						 glm::value_ptr(quad_model_matrix));
		
		if(check_ogl_error()){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to set model matrix for spot light!" << std::endl;
			errorlogger("ERROR: Failed to set model matrix for spot light!");
			return false;
		}

		glUniform3fv(shader->load_uniform_location("light.position"), 1, (float*)&(position));
		glUniform3fv(shader->load_uniform_location("light.direction"), 1, (float*)&(direction));
		glUniform1f(shader->load_uniform_location("light.cut_off"), cutoff);
		glUniform1f(shader->load_uniform_location("light.outer_cut_off"), outer_cutoff);
		glUniform1f(shader->load_uniform_location("light.radius"), radius);

		glUniform3fv(shader->load_uniform_location("light.color"), 1, (float*)&(color));
		glUniform3fv(shader->load_uniform_location("light.color_components"), 1, (float*)&(color_components));
		if(check_ogl_error()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind spot light uniforms!" << std::endl;
			errorlogger("ERROR: Failed to bind spot light uniforms!");
			return false;
		}

		return true;
	};

	return true;
}

bool Spot_light::calculate_light_uniforms(){

	/* TODO::Optimize based on direction */
	scale = {radius * 2, radius * 2, radius * 2};

	quad_model_matrix = glm::mat4();
	quad_model_matrix = glm::translate(quad_model_matrix, position);  
	quad_model_matrix = glm::scale(quad_model_matrix, scale);  

	return true;
}