#include "dir_light.h"

Directional_light::Directional_light(const glm::vec3& dir, 
									const glm::vec3& color, 
									GLboolean render_shadows){
	base_light_context->shader_type = LIGHT_DIRECTIONAL;
	
	this->direction = glm::normalize(dir);
	this->color = color;
	if (render_shadows) {
		this->render_shadows = 1;
	}
	else{
		this->render_shadows = 0;
	}
	apply_SSAO = 1;

	if (!bind_lambda_expression()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to bind lambda expression for directional light!" << std::endl;
		errorlogger("FATAL ERROR: Failed to bind lambda expression for directional light!");
		exit(EXIT_FAILURE);
	}
}

Directional_light::Directional_light(){
	base_light_context->shader_type = LIGHT_DIRECTIONAL;
	direction = {0.0f, -1.0f, -1.0f};
	//direction = glm::normalize(direction);
	color = {0.02f, 0.02f, 0.02f};
	render_shadows = 1;
	apply_SSAO = 1;

	if (!bind_lambda_expression()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to bind lambda expression for directional light!" << std::endl;
		errorlogger("FATAL ERROR: Failed to bind lambda expression for directional light!");
		exit(EXIT_FAILURE);
	}
}

bool Directional_light::bind_lambda_expression()const{
	base_light_context->setup_base_uniforms = [&](GLuint buffer, const glm::mat4& view, GLuint instance) {
		(void(view));
		GLuint base_offset = Utility_consts::SIZEOF_DIR_LIGHT * instance;
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(glm::vec3), glm::value_ptr(direction)); 
		base_offset += sizeof(glm::vec4);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(glm::vec3), glm::value_ptr(color));
		base_offset += sizeof(glm::vec4);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(GLint), (GLvoid*)(&render_shadows));
		base_offset += sizeof(GLuint);

		glBufferSubData(GL_UNIFORM_BUFFER, base_offset, sizeof(GLint), (GLvoid*)(&apply_SSAO));

		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		if(check_ogl_error()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind directional light uniforms!" << std::endl;
			errorlogger("ERROR: Failed to bind directional light uniforms!");
			return false;
		}

		return true;
	};

	return true;
}