#include "spot_light.h"

Spot_light::Spot_light(){
	base_light_context->shader_type = LIGHT_SPOT;
	direction = {0.0f, -1.0f, 0.0f};
	randomize_position(glm::i16vec3(4000, 100, 4000), glm::i16vec3(2000, 0, 2000));
	randomize_diffuse(glm::i16vec3(2.0f, 2.0f, 2.0f));
	randomize_specular(glm::i16vec3(2.0f, 2.0f, 2.0f));

	if (!calculate_light_uniforms(0.00014f, 0.000007f, 1.7f, 2.0f, 1.0f)) {
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
		glUniform1f(shader->load_uniform_location("light.cut_off"), cut_off);
		glUniform1f(shader->load_uniform_location("light.outer_cut_off"), outer_cut_off);
		glUniform1f(shader->load_uniform_location("light.linear"), linear);
		glUniform1f(shader->load_uniform_location("light.quadratic"), quadratic);

		glUniform3fv(shader->load_uniform_location("light.ambient"), 1, (float*)&(ambient));
		glUniform3fv(shader->load_uniform_location("light.diffuse"), 1, (float*)&(diffuse));
		glUniform3fv(shader->load_uniform_location("light.specular"), 1, (float*)&(specular));
		if(check_ogl_error()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind spot light uniforms!" << std::endl;
			errorlogger("ERROR: Failed to bind spot light uniforms!");
			return false;
		}

		return true;
	};

	return true;
}

bool Spot_light::calculate_light_uniforms(GLfloat linear, 
										GLfloat quadratic, 
										GLfloat cut_off,
										GLfloat outer_cut_off,
										GLfloat intensity){
	GLfloat C = glm::max(ambient.x + diffuse.x + specular.x, glm::max(
						ambient.y + diffuse.y + specular.y,
						ambient.z + diffuse.z + specular.z));

	GLfloat distance = (-linear + (sqrt((linear*linear) - ((4 * quadratic) * (1 - (256 * C * intensity))))));
	distance /= 2*quadratic;

	scale = {distance, distance, distance};

	quad_model_matrix = glm::mat4();
	quad_model_matrix = glm::translate(quad_model_matrix, position);  
	quad_model_matrix = glm::scale(quad_model_matrix, scale);  

	return true;
}