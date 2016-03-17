#include "dir_light.h"

Directional_light::Directional_light(){
	type = DIRECTIONAL;
	direction = {0.0f, -1.0f, 0.0f};
	size = {1.0f, 1.0f, 1.0f};
	ambient = {0.1f, 0.1f, 0.1f};
	diffuse = {0.1f, 0.1f, 0.1f};
	specular = {0.0f, 0.0f, 0.0f};

	scale = {1.0f, 1.0f, 1.0f};
		
	if(!init_light_quad()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize geometry quad in dir light!" << std::endl;
		errorlogger("ERROR: Failed to initialize geometry quad in dir light!");
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

	GLuint quad_VBO;
	glGenVertexArrays(1, &clip_VAO);
	glGenBuffers(1, &quad_VBO);
	glBindVertexArray(clip_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bind geometry VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to geometry VAO in renderer!");
		glDeleteBuffers(1, &quad_VBO);
		glDeleteVertexArrays(1, &clip_VAO);
		return false;
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to buffer vertex data for geometry VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to buffer vertex data for geometry VAO in renderer!");
		glDeleteBuffers(1, &quad_VBO);
		glDeleteVertexArrays(1, &clip_VAO);
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
		glDeleteVertexArrays(1, &clip_VAO);
		return false;
	}
	return true;
}


bool Directional_light::render_light_quad()const{
	glBindVertexArray(clip_VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render light quad for directional light!" << std::endl;
		errorlogger("ERROR: Failed to render light quad for directional light!");
		return false;
	}
	return true;
}

bool Directional_light::render_light(const Shader_ptr& shader)const{
	if (!shader) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: ERROR: Null shader passed when rendering directional light!" << std::endl;
		errorlogger("ERROR: Null shader passed when rendering directional light!");
		return false;
	}

	glUniform3fv(shader->load_uniform_location("dir_light.direction"), 1, (float*)&(direction));
	glUniform3fv(shader->load_uniform_location("dir_light.ambient"), 1, (float*)&(ambient));
	glUniform3fv(shader->load_uniform_location("dir_light.diffuse"), 1, (float*)&(diffuse));
	glUniform3fv(shader->load_uniform_location("dir_light.specular"), 1, (float*)&(specular));
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind directional light uniforms!" << std::endl;
		errorlogger("ERROR: Failed to bind directional light uniforms!");
		return false;
	}

	render_light_quad();

	return true;
}
