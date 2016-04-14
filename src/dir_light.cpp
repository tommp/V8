#include "dir_light.h"

Directional_light::Directional_light(const glm::vec3& dir, 
									const glm::vec3& color, 
									const glm::vec3& color_components){
	base_light_context->shader_type = LIGHT_DIRECTIONAL;
	this->direction = dir;
	this->color = color;
	this->color_components = color_components;

	render_shadows = false;
	stepsize = 5.0;
	shadow_slack = 15.0;
	loop_offset = 4.0;
	probe_length = 100.0;

	if (!bind_lambda_expression()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to bind lambda expression for directional light!" << std::endl;
		errorlogger("FATAL ERROR: Failed to bind lambda expression for directional light!");
		exit(EXIT_FAILURE);
	}
}

Directional_light::Directional_light(){
	base_light_context->shader_type = LIGHT_DIRECTIONAL;
	direction = {-1.0f, -1.0f, -1.0f};
	direction = glm::normalize(direction);
	color = {0.1f, 0.1f, 0.1f};
	color_components = {1.0f, 1.0f, 0.0f};

	render_shadows = true;
	stepsize = 10.0;
	shadow_slack = 0;
	loop_offset = 1;
	probe_length = 2000.0;

	if (!bind_lambda_expression()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to bind lambda expression for directional light!" << std::endl;
		errorlogger("FATAL ERROR: Failed to bind lambda expression for directional light!");
		exit(EXIT_FAILURE);
	}
}

bool Directional_light::bind_lambda_expression()const{
	base_light_context->setup_base_uniforms = [&](const Shader_ptr& shader, const glm::mat4& view, GLuint instance) {
		if (!shader) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: ERROR: Null shader passed when rendering directional light!" << std::endl;
			errorlogger("ERROR: Null shader passed when rendering directional light!");
			return false;
		}

		glUniform3fv(shader->load_uniform_location("lights", instance, "direction"), 1, (float*)&(direction));
		glUniform3fv(shader->load_uniform_location("lights", instance, "color"), 1, (float*)&(color));
		glUniform3fv(shader->load_uniform_location("lights", instance, "color_components"), 1, (float*)&(color_components));
		
		glUniform1i(shader->load_uniform_location("lights", instance, "render_shadows"), render_shadows);
		if (render_shadows) {
			glUniform1f(shader->load_uniform_location("lights", instance, "stepsize"), stepsize);
			glUniform1f(shader->load_uniform_location("lights", instance, "shadow_slack"), shadow_slack);
			glUniform1f(shader->load_uniform_location("lights", instance, "loop_offset"), loop_offset);

			GLfloat number_of_steps = probe_length / stepsize;
			glUniform1f(shader->load_uniform_location("lights", instance, "num_steps"), number_of_steps);
		}
		
		if(check_ogl_error()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind directional light uniforms!" << std::endl;
			errorlogger("ERROR: Failed to bind directional light uniforms!");
			return false;
		}

		return true;
	};

	return true;
}