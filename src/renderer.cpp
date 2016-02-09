#include "renderer.h"

Renderer::Renderer(){
	display = nullptr;
	g_buffer = 0;
	g_position = 0;
	g_normal = 0;
	g_albedo_spec = 0;
	g_rbo_depth = 0;
};

Renderer::Renderer(Display& display, Resource_manager& resource_manager){
	this->display = &display;

	if (!init_matrix_uniform_buffer()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize unform buffers!" << std::endl;
		errorlogger("ERROR: Failed to initialize unform buffers");
		exit(EXIT_FAILURE);
	}
	
	if(!init_light_quad()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize geometry quad in renderer!" << std::endl;
		errorlogger("ERROR: Failed to initialize geometry quad in renderer!");
		exit(EXIT_FAILURE);
	}

	if(!init_shaders(resource_manager)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize shaders in renderer!" << std::endl;
		errorlogger("ERROR: Failed to initialize shaders in renderer!");
		exit(EXIT_FAILURE);
	}

	if(!init_framebuffer()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize g_framebuffer in renderer!" << std::endl;
		errorlogger("ERROR: Failed to initialize g_framebuffer in renderer!");
		exit(EXIT_FAILURE);
	}
	
	/* Set projection matrix */
    display.update_projection_matrix();
    display.upload_projection_matrix(uniform_buffers["matrices"]);
}

bool Renderer::init_matrix_uniform_buffer(){
	GLuint uniform_buffer_matrices;
	glGenBuffers(1, &uniform_buffer_matrices);
	  
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_matrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, uniform_buffer_matrices, 0, 2 * sizeof(glm::mat4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize matrix uniform buffer in renderer! " << std::endl;
		errorlogger("ERROR: Failed to initialize matrix uniform buffer in renderer!");
		glDeleteBuffers(1, &uniform_buffer_matrices);
		return false;
	}

	uniform_buffers["matrices"] = uniform_buffer_matrices;
	return true;
}

bool Renderer::init_light_quad(){
	GLfloat quad_vertices[] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	GLuint quad_VBO;
	glGenVertexArrays(1, &quad_VAO);
	glGenBuffers(1, &quad_VBO);
	glBindVertexArray(quad_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bind geometry VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to geometry VAO in renderer!");
		glDeleteBuffers(1, &quad_VBO);
		glDeleteVertexArrays(1, &quad_VAO);
		return false;
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to buffer vertex data for geometry VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to buffer vertex data for geometry VAO in renderer!");
		glDeleteBuffers(1, &quad_VBO);
		glDeleteVertexArrays(1, &quad_VAO);
		return false;
	}
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to set vertex attributes for geometry VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to set vertex attributes for geometry VAO in renderer!");
		glDeleteBuffers(1, &quad_VBO);
		glDeleteVertexArrays(1, &quad_VAO);
		return false;
	}

	return true;
}

bool Renderer::init_shaders(Resource_manager& resource_manager){
	light_shader = resource_manager.load_shader("light_shader");
	if (!light_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load light shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load light shader in renderer");
		return false;
	}

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to set uniform values for g_buffer textures in renderer!" << std::endl;
		errorlogger("ERROR: Failed to set uniform values for g_buffer textures in renderer!");
		return false;
	}

	geometry_shader = resource_manager.load_shader("geometry_shader");
	if (!geometry_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load geometry shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load geometry shader in renderer");
		return false;
	}
	return true;
}

bool Renderer::init_framebuffer(){
	light_shader->use();
	if (!display) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Display not initialized, cannon initialize g buffer!" << std::endl;
		errorlogger("ERROR: Display not initialized, cannon initialize g buffer!");
		return false;
	}

	glGenFramebuffers(1, &g_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
	  
	// - Position color buffer
	glGenTextures(1, &g_position);
	glBindTexture(GL_TEXTURE_2D, g_position);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, display->get_screen_width(), display->get_screen_height(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_position, 0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize g_position buffer in g_buffer!" << std::endl;
		errorlogger("ERROR: Failed to initialize g_position buffer in g_buffer!");
		glDeleteTextures(1, &g_position);
		return false;
	}
	// - Normal color buffer
	glGenTextures(1, &g_normal);
	glBindTexture(GL_TEXTURE_2D, g_normal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, display->get_screen_width(), display->get_screen_height(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_normal, 0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize g_normal buffer in g_buffer!" << std::endl;
		errorlogger("ERROR: Failed to initialize g_normal buffer in g_buffer!");
		glDeleteTextures(1, &g_normal);
		return false;
	}

	// - Color + Specular color buffer
	glGenTextures(1, &g_albedo_spec);
	glBindTexture(GL_TEXTURE_2D, g_albedo_spec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, display->get_screen_width(), display->get_screen_height(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_albedo_spec, 0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize g_albedo_spec buffer in g_buffer!" << std::endl;
		errorlogger("ERROR: Failed to initialize g_albedo_spec buffer in g_buffer!");
		glDeleteTextures(1, &g_albedo_spec);
		return false;
	}

	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	glGenRenderbuffers(1, &g_rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, g_rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, display->get_screen_width(), display->get_screen_height());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, g_rbo_depth);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize g_rbo_depth buffer in g_buffer!" << std::endl;
		errorlogger("ERROR: Failed to initialize g_rbo_depth buffer in g_buffer!");
		glDeleteRenderbuffers(1, &g_rbo_depth);
		return false;
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		print_framebuffer_error_in_fucking_english();
		errorlogger( "ERROR: Framebuffer not complete!");
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (!set_clear_color_black()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to set clear color!" << std::endl;
		errorlogger("ERROR: Failed to set clear color!");
		return false;
	}

	return true;
}

bool Renderer::set_clear_color_black(){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to set clear color to black!" << std::endl;
		errorlogger("ERROR: Failed to set clear color to black!");
		return false;
	}
	return true;
}

bool Renderer::use_g_buffer()const{
	glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bind g_buffer!" << std::endl;
		errorlogger("ERROR: Failed to bind g_buffer!");
		return false;
	}
	return true;
}

bool Renderer::use_default_buffer()const{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bind default_framebuffer!" << std::endl;
		errorlogger("ERROR: Failed to bind default_framebuffer!");
		return false;
	}
	return true;
}

bool Renderer::bind_g_data()const{
	light_shader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_position);
	glUniform1i(glGetUniformLocation(light_shader->get_program(), "g_position"), 0);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind g_position buffer!" << std::endl;
		errorlogger("ERROR: Failed to bind g_position buffer!");
		return false;
	}

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, g_normal);
	glUniform1i(glGetUniformLocation(light_shader->get_program(), "g_normal"), 1);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind g_normal buffer!" << std::endl;
		errorlogger("ERROR: Failed to bind g_normal buffer!");
		return false;
	}
	
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, g_albedo_spec);
	glUniform1i(glGetUniformLocation(light_shader->get_program(), "g_albedo_spec"), 2);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind g_albedo_spec buffer!" << std::endl;
		errorlogger("ERROR: Failed to bind g_albedo_spec buffer!");
		return false;
	}
	return true;
}

bool Renderer::unbind_g_data()const{

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to unbind g_position buffer!" << std::endl;
		errorlogger("ERROR: Failed to unbind g_position buffer!");
		return false;
	}

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, 0);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to unbind g_normal buffer!" << std::endl;
		errorlogger("ERROR: Failed to unbind g_normal buffer!");
		return false;
	}
	
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, 0);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to unbind g_albedo_spec buffer!" << std::endl;
		errorlogger("ERROR: Failed to unbind g_albedo_spec buffer!");
		return false;
	}
	return true;
}

GLuint Renderer::get_uniform_buffer(const std::string& name)const{
	if(uniform_buffers.find(name) != uniform_buffers.end()){
		return uniform_buffers.find(name)->second;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Uniform buffer not availiable, query name: " << name << std::endl;
		errorlogger("ERROR: Uniform buffer not availiable, query name: ", name.c_str());
		exit(EXIT_FAILURE);
		return -1;
	}
};

bool Renderer::render_geometry(GLuint VAO, 
							GLuint num_vertices,
							const Material_ptr& material, 
							const glm::vec3& position, 
							const glm::vec3& size, 
							const glm::vec3& direction)const{
	/* Model matrix calculations */
	glm::mat4 model;
	model = glm::translate(model, position);  

	GLfloat dot = glm::dot(direction, MESH_DIRECTION);
	GLfloat det =  MESH_DIRECTION.x*direction.z - MESH_DIRECTION.z*direction.x;
	GLfloat rotation = -1 * glm::atan(det, dot);

    //model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z)); 
    model = glm::rotate(model, rotation, glm::vec3(0.0f, 1.0f, 0.0f)); 
    //model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.5f * size.z));

    model = glm::scale(model, glm::vec3(size)); 

    /* Set model matrix */
    geometry_shader->use_shader_and_set_matrix4("model", model);

    material->use(geometry_shader);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, num_vertices, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    glBindVertexArray(0);
    
    /* Check for errors */
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render mesh!" << std::endl;
		errorlogger("ERROR: Failed to render mesh!");
		return false;
	}
	return true;
}

bool Renderer::render_light_quad()const{
	glBindVertexArray(quad_VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    return true;
}

void Renderer::setup_light_rendering()const{
	use_default_buffer();
	clear_display();
	bind_g_data();
	glUniform3fv(glGetUniformLocation(light_shader->get_program(), "view_position"), 1, (float*)camera.get_position_pointer());
}

void Renderer::detach_light_rendering()const{
	render_light_quad();
	unbind_g_data();
}

void Renderer::setup_geometry_rendering(){
	update_view_matrix();
	upload_view_matrix(uniform_buffers.find("matrices")->second);
	use_g_buffer();
	clear_display();
}

void Renderer::detach_geometry_rendering()const{
	use_default_buffer();
}

void Renderer::center_camera(const Actor_ptr& target, GLuint bound_width, GLuint bound_height) {
		camera.center_camera(target, bound_width, bound_height);
	}