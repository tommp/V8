#include "point_light.h"

Point_light::Point_light(){
	type = POINT;
	position = {0.0f, 0.0f, 0.0f};
	size = {1.0f, 1.0f, 1.0f};
	position.x = rand() % 4001 - 2000;
	position.y = (rand() % 40) + 100;
	position.z = rand() % 4001 - 2000;

	ambient = {0.0f, 0.0f, 0.0f};
	diffuse = {0.0f, 0.0f, 0.0f};
	specular = {0.0f, 0.0f, 0.0f};

	diffuse.x = (rand()%1000) / 10.0f;

	diffuse.y = (rand()%1000) / 10.0f;

	diffuse.z = (rand()%1000) / 10.0f;

	specular.x = (rand()%1000) / 100.0f;

	specular.y = (rand()%1000) / 100.0f;

	specular.z = (rand()%1000) / 100.0f;

	linear = 0.00014f;
	quadratic = 0.01f;

	GLfloat C = glm::max(ambient.x + diffuse.x + specular.x, glm::max(
						ambient.y + diffuse.y + specular.y,
						ambient.z + diffuse.z + specular.z));

	GLfloat intensity = 1.0f;

	GLfloat distance = (-linear + (sqrt((linear*linear) - ((4 * quadratic) * (1 - (256 * C * intensity))))));
	distance /= 2*quadratic;

	scale = {distance, distance, distance};

	if(!init_light_quad()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize geometry quad in point light!" << std::endl;
		errorlogger("ERROR: Failed to initialize geometry quad in point light!");
		exit(EXIT_FAILURE);
	}

	rendering_context->model_matrix = glm::mat4();
	rendering_context->model_matrix = glm::translate(rendering_context->model_matrix, position);  
	rendering_context->model_matrix = glm::scale(rendering_context->model_matrix, glm::vec3(10.0, 10.0, 10.0)); 

	quad_model_matrix = glm::mat4();
	quad_model_matrix = glm::translate(quad_model_matrix, position);  
	quad_model_matrix = glm::scale(quad_model_matrix, scale); 

	rendering_context->init_direction = direction;
	
}

bool Point_light::init_light_quad(){
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

	GLuint quad_VBO;
	GLuint quad_EBO;
	glGenVertexArrays(1, &clip_VAO);
	glGenBuffers(1, &quad_VBO);
	glGenBuffers(1, &quad_EBO);
	glBindVertexArray(clip_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_EBO);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bind geometry VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to geometry VAO in renderer!");
		glDeleteBuffers(1, &quad_VBO);
		glDeleteVertexArrays(1, &clip_VAO);
		return false;
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), &quad_indices, GL_STATIC_DRAW);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to buffer vertex data for geometry VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to buffer vertex data for geometry VAO in renderer!");
		glDeleteBuffers(1, &quad_VBO);
		glDeleteVertexArrays(1, &clip_VAO);
		return false;
	}
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
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

bool Point_light::render_light_quad()const{
	glBindVertexArray(clip_VAO);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW); 
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDisable(GL_CULL_FACE);
	glBindVertexArray(0);
	return true;
}

bool Point_light::render_light(const Shader_ptr& shader)const{
	if (!shader) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Null shader passed when rendering point light!" << std::endl;
		errorlogger("ERROR: Null shader passed when rendering point light!");
		return false;
	}

	shader->set_matrix4("model", quad_model_matrix);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to set model matrix for point light!" << std::endl;
		errorlogger("ERROR: Failed to set model matrix for point light!");
		return false;
	}

	glUniform3fv(shader->load_uniform_location("point_light.position"), 1, (float*)&(position));
	glUniform1f(shader->load_uniform_location("point_light.linear"), linear);
	glUniform1f(shader->load_uniform_location("point_light.quadratic"), quadratic);

	glUniform3fv(shader->load_uniform_location("point_light.ambient"), 1, (float*)&(ambient));
	glUniform3fv(shader->load_uniform_location("point_light.diffuse"), 1, (float*)&(diffuse));
	glUniform3fv(shader->load_uniform_location("point_light.specular"), 1, (float*)&(specular));
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind point light uniforms!" << std::endl;
		errorlogger("ERROR: Failed to bind point light uniforms!");
		return false;
	}
	render_light_quad();

	return true;
}