#include "point_light.h"

Point_light::Point_light(){
	base_light_context->shader_type = LIGHT_POINT;
	randomize_position(glm::i16vec3(4000, 100, 4000), glm::i16vec3(2000, 0, 2000));
	randomize_diffuse(glm::i16vec3(2.0f, 2.0f, 2.0f));
	randomize_specular(glm::i16vec3(2.0f, 2.0f, 2.0f));

	if (!calculate_light_uniforms(0.00014f, 0.01f, 1.0f)) {
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
		glUniform1f(shader->load_uniform_location("light.linear"), linear);
		glUniform1f(shader->load_uniform_location("light.quadratic"), quadratic);

		glUniform3fv(shader->load_uniform_location("light.ambient"), 1, (float*)&(ambient));
		glUniform3fv(shader->load_uniform_location("light.diffuse"), 1, (float*)&(diffuse));
		glUniform3fv(shader->load_uniform_location("light.specular"), 1, (float*)&(specular));
		if(check_ogl_error()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind point light uniforms!" << std::endl;
			errorlogger("ERROR: Failed to bind point light uniforms!");
			return false;
		}

		return true;
	};

	return true;
}

bool Point_light::calculate_light_uniforms(GLfloat linear, GLfloat quadratic, GLfloat intensity){
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