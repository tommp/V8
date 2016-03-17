#include "base_geometry.h"

Base_geometry::~Base_geometry() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &(base_context->VAO));
}

Base_geometry::Base_geometry() {
	base_context = std::make_shared<Base_render_context>();
	base_context->object_color = {1.0f, 1.0f, 1.0f, 0.0f};
	base_context->render_mode = GL_FILL;
	base_context->shader_type = GEOMETRY_STATIC_COLORED;
	base_context->material = nullptr;
}

Base_geometry::Base_geometry(Geom_type type, const glm::vec4& color) {
	base_context = std::make_shared<Base_render_context>();
	base_context->object_color = color;
	base_context->render_mode = GL_FILL;
	base_context->shader_type = GEOMETRY_STATIC_COLORED;
	base_context->material = nullptr;

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
		base_context->num_vertices = 24;
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

		base_context->num_vertices = 2;
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
	glGenVertexArrays(1, &(base_context->VAO));
	glGenBuffers(1, &VBO);
  
	glBindVertexArray(base_context->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, base_context->num_vertices * 6 * sizeof(GLfloat), 
				 &vertices[0], GL_STATIC_DRAW);/* TODO::CHANGE STATIC DRAW?? */

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

bool Base_geometry::add_base_to_context(Rendering_context_ptr& context)const{
	Base_render_context_weak weak_context = base_context;
	context->base_contexts.push_back(weak_context);
	return true;
}