#include "point_light.h"

Point_light::Point_light(GLfloat radius, 
						const glm::vec3& pos, 
						const glm::vec3& color, 
						GLboolean render_shadows) {
	base_light_context->shader_type = LIGHT_POINT;

	this->radius = radius;
	this->color = color;
	this->position = pos;
	if (render_shadows) {
		this->render_shadows = 1;
	}
	else{
		this->render_shadows = 0;
	}
	apply_SSAO = 1;

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
	randomize_position(glm::i16vec3(1000, 10, 1000), glm::i16vec3(500, -70, 500));
	randomize_color(5);
	render_shadows = 0;
	apply_SSAO = 0;
	

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
	base_light_context->setup_base_uniforms = [&](GLuint buffer, const glm::mat4& transform, const glm::mat3& screen_transform, GLuint instance) {
		
		glm::vec4 proj_position = transform * glm::vec4(position, 1.0);
		proj_position /= proj_position.w;
		glm::vec3 true_position = screen_transform * glm::vec3(proj_position.x, proj_position.y, proj_position.z);

		GLuint base_offset = Utility_consts::SIZEOF_POINT_LIGHT * instance;
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(glm::mat4), glm::value_ptr(transform * quad_model_matrix));
		base_offset += sizeof(glm::mat4);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(glm::vec3), glm::value_ptr(true_position));
		base_offset += sizeof(glm::vec4);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(glm::vec3), glm::value_ptr(color));
		base_offset += sizeof(glm::vec4);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(GLfloat), (GLvoid*)(&radius));
		base_offset += sizeof(GLfloat);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(GLuint), (GLvoid*)(&render_shadows));
		base_offset += sizeof(GLuint);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(GLuint), (GLvoid*)(&apply_SSAO));
		
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		if(check_ogl_error()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload point light uniforms!" << std::endl;
			errorlogger("ERROR: Failed to upload point light uniforms!");
			return false;
		}

		return true;
	};

	return true;
}

bool Point_light::calculate_light_uniforms(){

	scale = {radius, radius, radius};

	quad_model_matrix = glm::mat4();
	quad_model_matrix = glm::translate(quad_model_matrix, position);  
	quad_model_matrix = glm::scale(quad_model_matrix, scale); 


	return true;
}