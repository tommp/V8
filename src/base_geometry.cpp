#include "base_geometry.h"

Base_geometry::~Base_geometry() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &(rendering_context->VAO));
}

Base_geometry::Base_geometry() {
	rendering_context = std::make_shared<Rendering_context>();
	rendering_context->active = true;
	rendering_context->object_color = {1.0f, 1.0f, 1.0f, 1.0f};
	rendering_context->render_mode = GL_FILL;
	rendering_context->shader_type = GEOMETRY_STATIC;
	rendering_context->material = nullptr;
}

Base_geometry::Base_geometry(Geom_type type, const glm::vec4& color) {
	rendering_context = std::make_shared<Rendering_context>();
	rendering_context->active = true;
	rendering_context->object_color = color;
	rendering_context->render_mode = GL_FILL;
	rendering_context->shader_type = GEOMETRY_STATIC;
	rendering_context->material = nullptr;

	if (type == BOX) {
		GLfloat vertices[] = {
			-1.0f,  -1.0f,  1.0f,  0.0f,  -1.0f,  0.0f, 
			-1.0f,  1.0f,  1.0f,  -1.0f,  0.0f,  0.0f, 
			-1.0f,  -1.0f,  -1.0f,  0.0f,  -1.0f,  0.0f, 
			-1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,  -1.0f, 
			1.0f,  -1.0f,  1.0f,  0.0f,  -1.0f,  0.0f, 
			1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 
			1.0f,  -1.0f,  -1.0f,  0.0f,  -1.0f,  0.0f, 
			1.0f,  1.0f,  -1.0f,  1.0f,  0.0f,  0.0f, 
			-1.0f,  -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 
			-1.0f,  -1.0f,  1.0f,  -1.0f,  0.0f,  0.0f, 
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 
			-1.0f,  1.0f,  -1.0f,  0.0f,  1.0f,  0.0f, 
			-1.0f,  1.0f,  -1.0f,  -1.0f,  0.0f,  0.0f, 
			-1.0f,  -1.0f,  -1.0f,  -1.0f,  0.0f,  0.0f, 
			-1.0f,  -1.0f,  -1.0f,  0.0f,  0.0f,  -1.0f, 
			1.0f,  1.0f,  -1.0f,  0.0f,  1.0f,  0.0f, 
			1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,  -1.0f, 
			1.0f,  -1.0f,  -1.0f,  0.0f,  0.0f,  -1.0f, 
			1.0f,  -1.0f,  -1.0f,  1.0f,  0.0f,  0.0f, 
			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 
			1.0f,  -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 
			1.0f,  -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 
		};
		rendering_context->num_vertices = 24;
		if (!gen_arrays(vertices)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to initialize Base_geometry for box!" << std::endl;
			errorlogger("FATAL ERROR: Failed to initialize Base_geometry for box!");
			exit(EXIT_FAILURE);
		}
	}

    else if(type == LINE) {
		GLfloat vertices[] = {
			-1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, 
			1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, 
		};

		rendering_context->num_vertices = 2;
		if (!gen_arrays(vertices)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to initialize Base_geometry for line!" << std::endl;
			errorlogger("FATAL ERROR: Failed to initialize Base_geometry for line!");
			exit(EXIT_FAILURE);
		}
	}

    else if (type == SPHERE){
		GLfloat vertices[] = {0};
	}

	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Invalid base geometry type!" << std::endl;
		errorlogger("FATAL ERROR: Invalid base geometry type!");
		exit(EXIT_FAILURE);
	}
}

bool Base_geometry::gen_arrays(GLfloat* vertices) {
	glGenVertexArrays(1, &(rendering_context->VAO));
	glGenBuffers(1, &VBO);
  
	glBindVertexArray(rendering_context->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, rendering_context->num_vertices * 6 * sizeof(GLfloat), 
				 vertices, GL_STATIC_DRAW);/* TODO::CHANGE STATIC DRAW?? */

	/* Position attribute */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	/* Normal attribute */
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); 

	/* Check for errors */
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to initialize Base_geometry!" << std::endl;
		errorlogger("ERROR: Failed to initialize Base_geometry!");
		return false;
	}
	return true;
}