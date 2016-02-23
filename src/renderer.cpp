#include "renderer.h"

Renderer::Renderer(){
};

bool Renderer::init_settings(){
	if (!load_settings()) {
		ortographic = false;
		mouse_visible = true;
		use_vsync = true;
		use_fullscreen = false;
		window_size.x = (GLfloat)SCREEN_WIDTH;
		window_size.y = (GLfloat)SCREEN_HEIGHT;

		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to load display settings, restoring defaults." << std::endl;
		errorlogger("ERROR: Failed to load display settings, restoring defaults.");

		save_settings();
		return false;
	}
	SDL_DisableScreenSaver();
	clear();
	return true;
}

Renderer::Renderer(Resource_manager& resource_manager){
	g_buffer = 0;
	g_position = 0;
	g_normal = 0;
	g_albedo_spec = 0;
	g_rbo_depth = 0;

	if(!init_settings()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to Initialize display settings! Reset to default." << std::endl;
		errorlogger("ERROR: Failed to Initialize display settings!");
	}

	/* Initialize the window */
	if(!init_window()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to Initialize display window!" << std::endl;
		errorlogger("ERROR: Failed to Initialize display window!");
		exit(EXIT_FAILURE);
	}

	/* Initialize opengl */
	if(!init_openGL()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to Initialize display openGL!" << std::endl;
		errorlogger("ERROR: Failed to Initialize display openGL!");
		exit(EXIT_FAILURE);
	}

	if (!init_uniform_buffers()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize unform buffers!" << std::endl;
		errorlogger("ERROR: Failed to initialize unform buffers");
		exit(EXIT_FAILURE);
	}

	if(!init_framebuffer()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize g_framebuffer in renderer!" << std::endl;
		errorlogger("ERROR: Failed to initialize g_framebuffer in renderer!");
		exit(EXIT_FAILURE);
	}

	if(!init_shaders(resource_manager)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize shaders in renderer!" << std::endl;
		errorlogger("ERROR: Failed to initialize shaders in renderer!");
		exit(EXIT_FAILURE);
	}

	if (!upload_light_data()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to upload light data in renderer!" << std::endl;
		errorlogger("ERROR: Failed to upload light data in renderer!");
		exit(EXIT_FAILURE);
	}
	
	/* Set projection matrix */
    update_projection_matrix();
    upload_projection_matrix();
}

bool Renderer::init_uniform_buffers(){
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

	GLuint uniform_buffer_light_data;
	glGenBuffers(1, &uniform_buffer_light_data);
	  
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_light_data);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 2, uniform_buffer_light_data, 0, sizeof(glm::vec2));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize liht data uniform buffer in renderer! " << std::endl;
		errorlogger("ERROR: Failed to initialize light data uniform buffer in renderer!");
		glDeleteBuffers(1, &uniform_buffer_matrices);
		return false;
	}

	uniform_buffers["light_data"] = uniform_buffer_light_data;
	return true;
}

bool Renderer::init_shaders(Resource_manager& resource_manager){
	dir_light_shader = resource_manager.load_shader("dir_light_shader");
	if (dir_light_shader == nullptr) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load directional light shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load directional light shader in renderer");
		return false;
	}

	if(!bind_g_data(DIRECTIONAL)){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bind g_data in renderer!" << std::endl;
		errorlogger("ERROR: Failed to bind g_data in renderer");
		return false;
	}


	point_light_shader = resource_manager.load_shader("point_light_shader");
	if (point_light_shader == nullptr) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load point light shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load point light shader in renderer");
		return false;
	}

	if(!bind_g_data(POINT)){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bind g_data in renderer!" << std::endl;
		errorlogger("ERROR: Failed to bind g_data in renderer");
		return false;
	}

	spot_light_shader = resource_manager.load_shader("spot_light_shader");
	if (spot_light_shader == nullptr) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load spot light shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load spot light shader in renderer");
		return false;
	}

	if(!bind_g_data(SPOT)){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bind g_data in renderer!" << std::endl;
		errorlogger("ERROR: Failed to bind g_data in renderer");
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
	//dir_light_shader->use();
	if ((window_size.x <= 0) || (window_size.y <= 0)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Display not initialized, cannot initialize g buffer!" << std::endl;
		errorlogger("ERROR: Display not initialized, cannot initialize g buffer!");
		return false;
	}

	glGenFramebuffers(1, &g_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
	  
	// - Position color buffer
	glGenTextures(1, &g_position);
	glBindTexture(GL_TEXTURE_2D, g_position);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window_size.x, window_size.y, 0, GL_RGBA, GL_FLOAT, NULL);
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window_size.x, window_size.y, 0, GL_RGBA, GL_FLOAT, NULL);
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window_size.x, window_size.y, 0, GL_RGBA, GL_FLOAT, NULL);
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
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window_size.x, window_size.y);
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

GLuint Renderer::get_light_shader_program(Light_type light_type)const{
	if(light_type ==  DIRECTIONAL){
		return dir_light_shader->get_program();
	}
	else if(light_type == POINT){
		return point_light_shader->get_program();
	}
	else if(light_type == SPOT){
		return spot_light_shader->get_program();
	}

	std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Invalid light type in get_shader_program()!" << std::endl;
	errorlogger("ERROR: Invalid light type in get_shader_program()!");
	return 0;
}

Shader_ptr Renderer::get_light_shader(Light_type light_type)const{
	if(light_type ==  DIRECTIONAL){
		return dir_light_shader;
	}
	else if(light_type == POINT){
		return point_light_shader;
	}
	else if(light_type == SPOT) {
		return spot_light_shader;
	}

	std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Invalid light type in get_light_shader()!" << std::endl;
	errorlogger("ERROR: Invalid light type in get_light_shader()!");
	return nullptr;
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

bool Renderer::use_light_shader(Light_type light_type)const{
	if(light_type == DIRECTIONAL) {
		dir_light_shader->use();
		return true;
	}

	else if(light_type == POINT) {
		point_light_shader->use();
		return true;
	}

	else if(light_type == SPOT) {
		spot_light_shader->use();
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Invalid light type when binding light shader!" << std::endl;
		errorlogger("ERROR: Invalid light type when binding light shader!");
		return false;
	}
}

bool Renderer::bind_g_data(Light_type light_type)const{
	GLuint program;
	if(light_type == DIRECTIONAL) {
		dir_light_shader->use();
		program = dir_light_shader->get_program();
	}

	else if(light_type == POINT) {
		point_light_shader->use();
		program = point_light_shader->get_program();
	}

	else if(light_type == SPOT) {
		spot_light_shader->use();
		program = spot_light_shader->get_program();
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Invalid light type when binding light shader!" << std::endl;
		errorlogger("ERROR: Invalid light type when binding light shader!");
		return false;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_position);
	glUniform1i(glGetUniformLocation(program, "g_position"), 0);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind g_position buffer!" << std::endl;
		errorlogger("ERROR: Failed to bind g_position buffer!");
		return false;
	}

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, g_normal);
	glUniform1i(glGetUniformLocation(program, "g_normal"), 1);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind g_normal buffer!" << std::endl;
		errorlogger("ERROR: Failed to bind g_normal buffer!");
		return false;
	}
	
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, g_albedo_spec);
	glUniform1i(glGetUniformLocation(program, "g_albedo_spec"), 2);
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

void Renderer::setup_geometry_rendering(const Camera_ptr& camera){
	update_view_matrix(camera->get_position_refrence(), 
						camera->get_target_refrence(), 
						camera->get_up_dir_refrence());
	upload_view_matrix();
	use_g_buffer();
	glDepthMask(GL_TRUE);
	clear();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	geometry_shader->use();
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to setup geometry rendering!" << std::endl;
		errorlogger("ERROR: Failed to setup geometry rendering!");
		exit(EXIT_FAILURE);
	}
}

bool Renderer::render_geometry(std::vector<const std::list<Character_ptr>*> targets, const Camera_ptr& camera){
	setup_geometry_rendering(camera);
	for (auto target_vector : targets) {
		for (auto character : (*target_vector)){
			character->render_frame(*this);
		}
	}
	detach_geometry_rendering();
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render geometry!" << std::endl;
		errorlogger("ERROR: Failed to render geometry!");
		exit(EXIT_FAILURE);
	}
	return true;
}

void Renderer::detach_geometry_rendering()const{
	glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
   	glBlendEquation(GL_FUNC_ADD);
   	glBlendFunc(GL_ONE, GL_ONE);
	use_default_buffer();
	clear();
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to detach geometry rendering!" << std::endl;
		errorlogger("ERROR: Failed to detach geometry rendering!");
		exit(EXIT_FAILURE);
	}

}



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
    geometry_shader->set_matrix4("model", model);

    if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to set model matrix!" << std::endl;
		errorlogger("ERROR: Failed to set model matrix!");
		return false;
	}

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

void Renderer::setup_light_rendering(Light_type light_type, const glm::vec3& position)const{
	use_light_shader(light_type);
	upload_view_position(get_light_shader_program(light_type), 
								position);
	bind_g_data(light_type);
}

void Renderer::upload_view_position(GLuint shader_program, const glm::vec3& position)const{
	glUniform3fv(glGetUniformLocation(shader_program, "view_position"), 1, (float*)&position);
}

bool Renderer::upload_light_data()const{
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffers.find("light_data")->second);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec2), glm::value_ptr(window_size));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);  

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload light data vector!" << std::endl;
		errorlogger("ERROR: Failed to upload light data vector!");
		return false;
	}
	return true;
}

//DISPFUNCS

void Renderer::update_projection_matrix(){
	if (ortographic) {
		projection = glm::ortho(0.0f, window_size.x, 0.0f, window_size.y, 0.1f, 3000.0f);
	}
	else{
		projection = glm::perspective(45.0f, window_size.x / window_size.y, 0.1f, 3000.0f);
	}
}

void Renderer::upload_projection_matrix()const{
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffers.find("matrices")->second);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);  

	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload projection matix!" << std::endl;
		errorlogger("ERROR: Failed to upload projection matix!");
		exit(EXIT_FAILURE);
	}
}

bool Renderer::save_settings(){
	std::ofstream contentf (DISPLAY_SETTINGS_FILE_PATH, std::ios::binary);

	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for display settings!" << std::endl;
		errorlogger("ERROR: Failed to open content file for display settings!");
		return false;
	}

	contentf.write(reinterpret_cast<char *>(&ortographic), sizeof(bool));
	contentf.write(reinterpret_cast<char *>(&use_vsync), sizeof(bool));
	contentf.write(reinterpret_cast<char *>(&use_fullscreen), sizeof(bool));
	contentf.write(reinterpret_cast<char *>(&mouse_visible), sizeof(bool));

	contentf.write(reinterpret_cast<char *>(&window_size.x), sizeof(GLfloat));
	contentf.write(reinterpret_cast<char *>(&window_size.y), sizeof(GLfloat));

	contentf.close();

	return true;
}

bool Renderer::load_settings(){
	std::ifstream contentf (DISPLAY_SETTINGS_FILE_PATH, std::ios::binary);

	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for display settings!" << std::endl;
		errorlogger("ERROR: Failed to open content file for display settings!");
		return false;
	}

	contentf.read(reinterpret_cast<char *>(&ortographic), sizeof(bool));
	contentf.read(reinterpret_cast<char *>(&use_vsync), sizeof(bool));
	contentf.read(reinterpret_cast<char *>(&use_fullscreen), sizeof(bool));
	contentf.read(reinterpret_cast<char *>(&mouse_visible), sizeof(bool));

	contentf.read(reinterpret_cast<char *>(&window_size.x), sizeof(GLfloat));
	contentf.read(reinterpret_cast<char *>(&window_size.y), sizeof(GLfloat));

	contentf.close();

	return true;
}

void Renderer::toggle_mouse()const{
	if (mouse_visible) {
		SDL_ShowCursor(0);
	}
	else{
		SDL_ShowCursor(1);
	}
}

bool Renderer::init_window(){
	/* Set to enable opengl window context */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);

	/*Initializes a window to render graphics in*/
	window = SDL_CreateWindow("V8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_size.x, window_size.y, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (window == nullptr){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to create SDL window, see errorlog for details."<<std::endl;
		SDLerrorLogger("SDL_CreateWindow");
		return false;
	}

	/* Create opengl context */
	gl_context = SDL_GL_CreateContext(window);
	if(gl_context == NULL){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
		SDLerrorLogger("SDL_GL_CreateContext");
		return false;
	}

	if(use_fullscreen){
		enable_fullscreen();
	}

	if(use_vsync) {
		enable_vsync();
	}

	if (!mouse_visible) {
		SDL_ShowCursor(0);
	}
	else{
		SDL_ShowCursor(1);
	}

	return true;
}

bool Renderer::enable_fullscreen(){
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	GLint width;
	GLint height;
	SDL_GetWindowSize(window, &width, &height);
	window_size.x = (GLfloat) width;
	window_size.y = (GLfloat) height;
	update_projection_matrix();
	upload_projection_matrix();
	return true;
}

bool Renderer::enable_vsync(){
	if ( !(SDL_GL_SetSwapInterval(1) < 0)){
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: SDL_GL_SetSwapInterval could not enable vsync! SDL Error: " << SDL_GetError() << std::endl;
		SDLerrorLogger("SDL_GL_SetSwapInterval");
		return false;
	}
}

bool Renderer::disable_vsync(){
	if ( !(SDL_GL_SetSwapInterval(0) < 0)){
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: SDL_GL_SetSwapInterval could not disable vsync! SDL Error: " << SDL_GetError() << std::endl;
		SDLerrorLogger("SDL_GL_SetSwapInterval");
		return false;
	}
}

bool Renderer::init_openGL(){
	/* Set this to true so GLEW knows to use a modern approach to retrieving 
	function pointers and extensions*/
	glewExperimental = GL_TRUE;

	/* Initialize GLEW to setup the OpenGL Function pointers */
	GLenum err = glewInit();
	if (err){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to Initialize GLEW in Display::init_openGL()!" << std::endl;
		errorlogger("ERROR: Failed to Initialize GLEW in Display::init_openGL()!");
		return false;
	}
	/* Discard all errors set by glewinit */
	check_ogl_error();

	/* Define the viewport dimensions
	glViewport(0, 0, width, height);
	if(check_ogl_error()) {
		std::cout << "ERROR: Failed to Initialize viewport in Display::init_openGL(): " << glewGetErrorString(err) << std::endl;
		errorlogger("ERROR: Failed to Initialize viewport in Display::init_openGL(): ", (const char*)glewGetErrorString(err));
		return false;
	}*/

	/* Setup OpenGL options */
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to Initialize depth testing in Display::init_openGL()!" << std::endl;
		errorlogger("ERROR: Failed to Initialize depth testing in Display::init_openGL()!");
		return false;
	}

	/* Initialize clear color */
	glClearColor(CLEARCOLOR[0], CLEARCOLOR[1], CLEARCOLOR[2], CLEARCOLOR[3]);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to Initialize clearcolour in Display::init_openGL()!" << std::endl;
		errorlogger("ERROR: Failed to Initialize clearcolour in Display::init_openGL()!");
		return false;
	}

	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to Initialize openGL in Display::init_openGL()!" << std::endl;
		errorlogger("ERROR: Failed to Initialize openGL in Display::init_openGL()!");
		return false;
	}

	std::cout << "OpenGL initialized! Version: " << glGetString(GL_VERSION) << "\n\n\n\n\n" << std::endl;

	return true;
}

void Renderer::clear()const{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::present()const{
	SDL_GL_SwapWindow(window);
}

//CAMFUNCS

void Renderer::update_view_matrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& camera_up) {
	/*std::cout << "POS: " << position.x << " : " << position.y << " : " << position.z << std::endl;
	std::cout << "TARG: " << target.x << " : " << target.y << " : " << target.z << std::endl;
	std::cout << "CAM: " << camera_up.x << " : " << camera_up.y << " : " << camera_up.z << std::endl;*/
	view = glm::lookAt(position, target, camera_up);
}

void Renderer::upload_view_matrix()const{
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffers.find("matrices")->second);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);  

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload view matrix!" << std::endl;
		errorlogger("ERROR: Failed to upload view matrix!");
		exit(EXIT_FAILURE);
	}
}