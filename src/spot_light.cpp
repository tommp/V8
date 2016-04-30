#include "spot_light.h"

Spot_light::Spot_light(GLfloat radius, 
						const glm::vec3& pos,
						const glm::vec3& dir,
						const glm::vec3& color, 
						const GLfloat cutoff,
						const GLfloat outer_cutoff,
						GLfloat intensity,
						GLboolean render_shadows) {
	base_light_context->shader_type = LIGHT_SPOT;

	this->radius = radius;
	this->position = pos;
	this->direction = dir;
	this->color = color;
	this->cutoff = cutoff;
	this->outer_cutoff = outer_cutoff;
	this->intensity = intensity;
	if (render_shadows) {
		this->render_shadows = 1;
	}
	else{
		this->render_shadows = 0;
	}
	this->apply_SSAO = 0;

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

	radius = (rand() % 400) + 200;

	cutoff = (rand() % 35) + 15;
	outer_cutoff = cutoff + (rand() % 10) + 10;

	render_shadows = 0;
	apply_SSAO = 0;

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
	base_light_context->setup_base_uniforms = [&](GLuint buffer, const glm::mat4& transform, const glm::mat3& screen_transform, GLuint instance) {
		
		glm::vec4 proj_position = transform * glm::vec4(position, 1.0);
		proj_position /= proj_position.w;
		glm::vec3 true_position = screen_transform * glm::vec3(proj_position.x, proj_position.y, proj_position.z);

		GLuint base_offset = Utility_consts::SIZEOF_SPOT_LIGHT * instance;
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(glm::mat4), glm::value_ptr(transform * quad_model_matrix));
		base_offset += sizeof(glm::mat4);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(glm::vec3), glm::value_ptr(true_position));
		base_offset += sizeof(glm::vec4);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(glm::vec3), glm::value_ptr(direction));
		base_offset += sizeof(glm::vec4);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(glm::vec3), glm::value_ptr(color));
		base_offset += sizeof(glm::vec4);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(GLfloat), (GLvoid*)(&radius));
		base_offset += sizeof(GLfloat);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(GLfloat), (GLvoid*)(&cutoff));
		base_offset += sizeof(GLfloat);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(GLfloat), (GLvoid*)(&outer_cutoff));
		base_offset += sizeof(GLfloat);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(GLuint), (GLvoid*)(&render_shadows));
		base_offset += sizeof(GLuint);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(GLuint), (GLvoid*)(&apply_SSAO));
		
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
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