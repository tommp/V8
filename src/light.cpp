#include "light.h"

Directional_light::Directional_light(){
	type = DIRECTIONAL;
	direction = {1.0f, -1.0f, 1.0f};
	size = {1.0f, 1.0f, 1.0f};
	ambient = {0.2f, 0.2f, 0.2f};
	diffuse = {0.01f, 0.01f, 0.01f};
	specular = {0.0f, 0.0f, 0.0f};

	scale = {1.0f, 1.0f, 1.0f};
		
	if(!init_light_quad()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize geometry quad in dir light!" << std::endl;
		errorlogger("ERROR: Failed to initialize geometry quad in dir light!");
		exit(EXIT_FAILURE);
	}
}

Point_light::Point_light(){
	type = POINT;
	position = {0.0f, 10.0f, 0.0f};
	size = {1.0f, 1.0f, 1.0f};
	position.x = rand() % 4001;
	position.z = rand() % 4001;

	ambient = {0.0f, 0.0f, 0.0f};
	diffuse = {0.0f, 0.0f, 0.0f};
	specular = {0.0f, 0.0f, 0.0f};

	diffuse.x = (rand()%1000) / 1000.0f;

	diffuse.y = (rand()%1000) / 1000.0f;

	diffuse.z = (rand()%1000) / 1000.0f;

	specular.x = (rand()%1000) / 1000.0f;

	specular.y = (rand()%1000) / 1000.0f;

	specular.z = (rand()%1000) / 1000.0f;

	linear = 0.00014f;
	quadratic = 0.0001f;

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
}

Spot_light::Spot_light(){
	type = SPOT;
	direction = {0.0f, -1.0f, 0.0f};
	position = {0.0f, 200.0f, 0.0f};
	size = {1.0f, 1.0f, 1.0f};
	position.x = rand() % 4001;
	position.z = rand() % 4001;

	ambient = {0.2f, 0.2f, 0.2f};
	diffuse = {10.0f, 10.0f, 10.0f};
	specular = {1.0f, 1.0f, 1.0f};

	linear = 0.0014f;
	quadratic = 0.000007f;

	cut_off = 1.7f;
	outer_cut_off = 2.0f;

	GLfloat C = glm::max(ambient.x + diffuse.x + specular.x, glm::max(
						ambient.y + diffuse.y + specular.y,
						ambient.z + diffuse.z + specular.z));

	GLfloat intensity = 1.0f;

	GLfloat distance = (-linear + (sqrt((linear*linear) - ((4 * quadratic) * (1 - (256 * C * intensity))))));
	distance /= 2*quadratic;

	scale = {distance, distance, distance};

	if(!init_light_quad()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize geometry quad in spot light!" << std::endl;
		errorlogger("ERROR: Failed to initialize geometry quad in spot light!");
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

bool Spot_light::init_light_quad(){
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

bool Point_light::init_light_quad(){
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
	return true;
}

bool Point_light::render_light_quad()const{
	glBindVertexArray(clip_VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	return true;
}

bool Spot_light::render_light_quad()const{
	glBindVertexArray(clip_VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	return true;
}

void Directional_light::render_light(const Renderer& renderer)const{

	GLuint program = renderer.get_light_shader_program(DIRECTIONAL);
	if (!program) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to get shader program when rendering directional light!" << std::endl;
		errorlogger("ERROR: Failed to get shader program when rendering directional light!");
	}

	glUniform3fv(glGetUniformLocation(program, "dir_light.direction"), 1, (float*)&(direction));
	glUniform3fv(glGetUniformLocation(program, "dir_light.ambient"), 1, (float*)&(ambient));
	glUniform3fv(glGetUniformLocation(program, "dir_light.diffuse"), 1, (float*)&(diffuse));
	glUniform3fv(glGetUniformLocation(program, "dir_light.specular"), 1, (float*)&(specular));
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind directional light uniforms!" << std::endl;
		errorlogger("ERROR: Failed to bind directional light uniforms!");
	}
	render_light_quad();
 }

void Point_light::render_light(const Renderer& renderer)const{
	glm::mat4 model;
	model = glm::translate(model, position);  
	model = glm::scale(model, glm::vec3(scale)); 

	renderer.get_light_shader(POINT)->set_matrix4("model", model);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to set model matrix for point light!" << std::endl;
		errorlogger("ERROR: Failed to set model matrix for point light!");
	}

	GLuint program = renderer.get_light_shader_program(POINT);
	if (!program) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to get shader program when rendering directional light!" << std::endl;
		errorlogger("ERROR: Failed to get shader program when rendering directional light!");
	}

	glUniform3fv(glGetUniformLocation(program, "point_light.position"), 1, (float*)&(position));
	glUniform1f(glGetUniformLocation(program, "point_light.linear"), linear);
	glUniform1f(glGetUniformLocation(program, "point_light.quadratic"), quadratic);

	glUniform3fv(glGetUniformLocation(program, "point_light.ambient"), 1, (float*)&(ambient));
	glUniform3fv(glGetUniformLocation(program, "point_light.diffuse"), 1, (float*)&(diffuse));
	glUniform3fv(glGetUniformLocation(program, "point_light.specular"), 1, (float*)&(specular));
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind point light uniforms!" << std::endl;
		errorlogger("ERROR: Failed to bind point light uniforms!");
	}
	render_light_quad();
}

void Spot_light::render_light(const Renderer& renderer)const{
	glm::mat4 model;
	model = glm::translate(model, position);  
	model = glm::scale(model, glm::vec3(size)); 

	renderer.get_light_shader(SPOT)->set_matrix4("model", model);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to set model matrix for spot light!" << std::endl;
		errorlogger("ERROR: Failed to set model matrix for spot light!");
	}

	GLuint program = renderer.get_light_shader_program(SPOT);
	if (!program) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to get shader program when rendering directional light!" << std::endl;
		errorlogger("ERROR: Failed to get shader program when rendering directional light!");
	}

	glUniform3fv(glGetUniformLocation(program, "spot_light.position"), 1, (float*)&(position));
	glUniform3fv(glGetUniformLocation(program, "spot_light.direction"), 1, (float*)&(direction));
	glUniform1f(glGetUniformLocation(program, "spot_light.cut_off"), cut_off);
	glUniform1f(glGetUniformLocation(program, "spot_light.outer_cut_off"), outer_cut_off);
	glUniform1f(glGetUniformLocation(program, "spot_light.linear"), linear);
	glUniform1f(glGetUniformLocation(program, "spot_light.quadratic"), quadratic);

	glUniform3fv(glGetUniformLocation(program, "spot_light.ambient"), 1, (float*)&(ambient));
	glUniform3fv(glGetUniformLocation(program, "spot_light.diffuse"), 1, (float*)&(diffuse));
	glUniform3fv(glGetUniformLocation(program, "spot_light.specular"), 1, (float*)&(specular));
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind spot light uniforms!" << std::endl;
		errorlogger("ERROR: Failed to bind spot light uniforms!");
	}
	render_light_quad();
}