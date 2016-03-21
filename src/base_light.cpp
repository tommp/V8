#include "base_light.h"

Base_light::Base_light(){
	ambient = {0.0f, 0.0f, 0.0f};
	diffuse = {0.0f, 0.0f, 0.0f};
	specular = {0.0f, 0.0f, 0.0f};
	scale = {100.0f, 100.0f, 100.0f};

	quad_model_matrix = glm::mat4();
	quad_model_matrix = glm::translate(quad_model_matrix, position);  
	quad_model_matrix = glm::scale(quad_model_matrix, scale); 

	base_light_context = std::make_shared<Rendering_context>();

	if (!init_light_volume()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to initialize light volume!!" << std::endl;
		errorlogger("FATAL ERROR: Failed to initialize light volume!");
		exit(EXIT_FAILURE);
	}
}

bool Base_light::init_light_volume(){
	GLfloat quad_vertices[] = {
		-1.0f, -1.0f, -1.0f, // 0
	     1.0f, -1.0f, -1.0f, // 1
	     1.0f,  1.0f, -1.0f, // 2
	    -1.0f,  1.0f, -1.0f, // 3
	    -1.0f, -1.0f,  1.0f, // 4
	     1.0f, -1.0f,  1.0f, // 5
	     1.0f,  1.0f,  1.0f, // 6
	    -1.0f,  1.0f,  1.0f // 7
	};

	GLuint quad_indices[] = {
		0,2,1,
        0,3,2,
        1,2,6,
        6,5,1,
        4,5,6,
        6,7,4,
        2,3,6,
        6,3,7,
        0,7,3,
        0,4,7,
        0,1,5,
        0,5,4,
	};

	base_light_context->num_vertices = 36;
	base_light_context->render_elements = true;
	base_light_context->shader_type = NO_SHADER;
	base_light_context->render_mode = GL_FILL;

	glGenVertexArrays(1, &base_light_context->VAO);
	glGenBuffers(1, &quad_VBO);
	glGenBuffers(1, &quad_EBO);
	glBindVertexArray(base_light_context->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_EBO);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bind geometry VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to geometry VAO in renderer!");
		glDeleteBuffers(1, &quad_VBO);
		glDeleteBuffers(1, &quad_EBO);
		glDeleteVertexArrays(1, &base_light_context->VAO);
		return false;
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), &quad_indices, GL_STATIC_DRAW);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to buffer vertex data for geometry VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to buffer vertex data for geometry VAO in renderer!");
		glDeleteBuffers(1, &quad_VBO);
		glDeleteBuffers(1, &quad_EBO);
		glDeleteVertexArrays(1, &base_light_context->VAO);
		return false;
	}
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to set vertex attributes for geometry VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to set vertex attributes for geometry VAO in renderer!");
		glDeleteBuffers(1, &quad_VBO);
		glDeleteBuffers(1, &quad_EBO);
		glDeleteVertexArrays(1, &base_light_context->VAO);
		return false;
	}
	return true;
}

bool Base_light::randomize_position(const glm::i16vec3& maxpos, const glm::i16vec3& offset){
	position.x = (rand() % (maxpos.x + 1)) - offset.x;
	position.y = (rand() % (maxpos.y + 1)) - offset.y;
	position.z = (rand() % (maxpos.z + 1)) - offset.z;

	return true;
}

bool Base_light::randomize_diffuse(){
	diffuse.x = (rand() % 1000) / 1000.0f;
	diffuse.y = (rand() % 1000) / 1000.0f;
	diffuse.z = (rand() % 1000) / 1000.0f;

	return true;
}

bool Base_light::randomize_ambient(){
	ambient.x = (rand() % 1000) / 1000.0f;
	ambient.y = (rand() % 1000) / 1000.0f;
	ambient.z = (rand() % 1000) / 1000.0f;

	return true;
}

bool Base_light::randomize_specular(){
	specular.x = (rand() % 1000) / 1000.0f;
	specular.y = (rand() % 1000) / 1000.0f;
	specular.z = (rand() % 1000) / 1000.0f;

	return true;
}

bool Base_light::free_buffers(){
	glDeleteBuffers(1, &quad_VBO);
	glDeleteBuffers(1, &quad_EBO);
	glDeleteVertexArrays(1, &base_light_context->VAO);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to free buffers in base light!" << std::endl;
		errorlogger("ERROR: Failed to free buffers in base light!");
		return false;
	}

	return true;
}

bool Base_light::add_context(Renderer& renderer)const{
	if (!renderer.add_context(base_light_context)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add light context to renderer!" << std::endl;
		errorlogger("ERROR: Failed to add light context to renderer!");
		return false;
	}

	return true;
}