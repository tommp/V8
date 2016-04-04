#include "dir_light.h"

Directional_light::Directional_light(const glm::vec3& dir, 
									const glm::vec3& color, 
									const glm::vec3& color_components){
	base_light_context->shader_type = LIGHT_DIRECTIONAL;
	this->direction = dir;
	this->color = color;
	this->color_components = color_components;

	if (!init_light_quad()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to initialize light quad!" << std::endl;
		errorlogger("FATAL ERROR: Failed to initialize light quad!");
		exit(EXIT_FAILURE);
	}

	if (!bind_lambda_expression()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to bind lambda expression for directional light!" << std::endl;
		errorlogger("FATAL ERROR: Failed to bind lambda expression for directional light!");
		exit(EXIT_FAILURE);
	}
}

Directional_light::Directional_light(){
	base_light_context->shader_type = LIGHT_DIRECTIONAL;
	direction = {0.0f, -1.0f, 1.0f};
	color = {0.3f, 0.3f, 0.3f};
	color_components = {1.0f, 1.0f, 0.0f};

	if (!init_light_quad()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to initialize light quad!" << std::endl;
		errorlogger("FATAL ERROR: Failed to initialize light quad!");
		exit(EXIT_FAILURE);
	}

	if (!bind_lambda_expression()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to bind lambda expression for directional light!" << std::endl;
		errorlogger("FATAL ERROR: Failed to bind lambda expression for directional light!");
		exit(EXIT_FAILURE);
	}
}

bool Directional_light::init_light_quad(){
	GLfloat quad_vertices[] = {
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	};

	free_buffers();

	base_light_context->num_vertices = 4;
	base_light_context->render_elements = false;
	base_light_context->render_mode = GL_FILL;

	glGenVertexArrays(1, &base_light_context->VAO);
	glGenBuffers(1, &quad_VBO);
	glBindVertexArray(base_light_context->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bind geometry VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to geometry VAO in renderer!");
		glDeleteBuffers(1, &quad_VBO);
		glDeleteVertexArrays(1, &base_light_context->VAO);
		return false;
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to buffer vertex data for geometry VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to buffer vertex data for geometry VAO in renderer!");
		glDeleteBuffers(1, &quad_VBO);
		glDeleteVertexArrays(1, &base_light_context->VAO);
		return false;
	}
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to set vertex attributes for geometry VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to set vertex attributes for geometry VAO in renderer!");
		glDeleteBuffers(1, &quad_VBO);
		glDeleteVertexArrays(1, &base_light_context->VAO);
		return false;
	}
	return true;
}

bool Directional_light::bind_lambda_expression()const{
	base_light_context->setup_base_uniforms = [&](const Shader_ptr& shader) {
		if (!shader) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: ERROR: Null shader passed when rendering directional light!" << std::endl;
			errorlogger("ERROR: Null shader passed when rendering directional light!");
			return false;
		}

		glUniform3fv(shader->load_uniform_location("light.direction"), 1, (float*)&(direction));
		glUniform3fv(shader->load_uniform_location("light.color"), 1, (float*)&(color));
		glUniform3fv(shader->load_uniform_location("light.color_components"), 1, (float*)&(color_components));
		if(check_ogl_error()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind directional light uniforms!" << std::endl;
			errorlogger("ERROR: Failed to bind directional light uniforms!");
			return false;
		}

		return true;
	};

	return true;
}