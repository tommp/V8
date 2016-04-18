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
	this->intensity = 1.0;

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
	intensity = 1.0;

	direction.x = ((rand() % 1500) / 501) - 1;
	direction.z = ((rand() % 1500) / 501) - 1;

	randomize_position(glm::i16vec3(1000, 50, 1000), glm::i16vec3(0, -100, 0));
	randomize_color(5);
	color_components = {0.0f, 1.0f, 1.0f};

	radius = (rand() % 400) + 200;

	cutoff = (rand() % 35) + 15;
	outer_cutoff = cutoff + (rand() % 10) + 10;

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
	base_light_context->setup_base_uniforms = [&](const Shader_ptr& shader, const glm::mat4& view, const glm::mat4& projection, GLuint near, GLuint far, GLuint instance) {
		if (!shader) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: ERROR: Null shader passed when rendering spot light!" << std::endl;
			errorlogger("ERROR: Null shader passed when rendering spot light!");
			return false;
		}

		glUniformMatrix4fv(shader->load_uniform_location("models", instance),
						 1, 
						 GL_FALSE, 
						 glm::value_ptr(quad_model_matrix));
		
		if(check_ogl_error()){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to set model matrix for spot light!" << std::endl;
			errorlogger("ERROR: Failed to set model matrix for spot light!");
			return false;
		}

		glm::vec3 view_position = glm::vec3(view * glm::vec4(position, 1.0));

		glUniform3fv(shader->load_uniform_location("lights", instance, "position"), 1, (float*)&(view_position));
		glUniform3fv(shader->load_uniform_location("lights", instance, "direction"), 1, (float*)&(direction));
		glUniform1f(shader->load_uniform_location("lights", instance, "cut_off"), cutoff);
		glUniform1f(shader->load_uniform_location("lights", instance, "outer_cut_off"), outer_cutoff);
		glUniform1f(shader->load_uniform_location("lights", instance, "radius"), radius);

		glm::vec3 scaled_color = color * intensity;

		glUniform3fv(shader->load_uniform_location("lights", instance, "color"), 1, (float*)&(scaled_color));
		glUniform3fv(shader->load_uniform_location("lights", instance, "color_components"), 1, (float*)&(color_components));
		
		glUniform1i(shader->load_uniform_location("lights", instance, "render_shadows"), render_shadows);
		if (render_shadows) {
			glUniform1f(shader->load_uniform_location("lights", instance, "stepsize"), stepsize);
			glUniform1f(shader->load_uniform_location("lights", instance, "shadow_slack"), shadow_slack);
			glUniform1f(shader->load_uniform_location("lights", instance, "loop_offset"), loop_offset);
		}

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
	scale = {radius, radius, radius};

	quad_model_matrix = glm::mat4();
	quad_model_matrix = glm::translate(quad_model_matrix, position);  
	quad_model_matrix = glm::scale(quad_model_matrix, scale);  

	return true;
}

bool Spot_light::set_intensity(GLfloat intensity){
	if (intensity > 1.0) {
		this->intensity = 1.0;
		return false;
	}
	else if (intensity < 0.0) {
		this->intensity = 0.0;
		return false;
	}

	this->intensity = intensity;

	return true;
}