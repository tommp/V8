#include "renderer.h"

Renderer::Renderer(){
};

Renderer::~Renderer() {
	delete_g_buffer();
	glDeleteBuffers(1, &uniform_buffer_matrices);
	glDeleteBuffers(1, &uniform_buffer_light_data);
}

Renderer::Renderer(Resource_manager& resource_manager){
	g_buffer = 0;
	g_position = 0;
	g_normal = 0;
	g_albedo_spec = 0;
	g_rbo_depth = 0;

	std::cout << "------------ Initializing renderer settings..." << std::endl;
	if (!init_settings()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Failed to Initialize display settings! Reset to default." << std::endl;
		errorlogger("WARNING: Failed to Initialize display settings!");
	}
	std::cout << "------------ Renderer settings initialized!\n" << std::endl;

	std::cout << "------------ Initializing renderer window..." << std::endl;
	if (!init_window()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to Initialize display window!" << std::endl;
		errorlogger("FATAL ERROR: Failed to Initialize display window!");
		exit(EXIT_FAILURE);
	}
	std::cout << "------------ Renderer window initialized!\n" << std::endl;

	std::cout << "------------ Initializing opengl..." << std::endl;
	/* Initialize opengl */
	if (!init_openGL()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to Initialize display openGL!" << std::endl;
		errorlogger("FATAL ERROR: Failed to Initialize display openGL!");
		exit(EXIT_FAILURE);
	}
	std::cout << "------------ OpenGL initialized! Version: " << glGetString(GL_VERSION) << "\n" << std::endl;

	std::cout << "------------ Initializing uniform buffers..." << std::endl;
	if (!init_uniform_buffers()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to initialize unform buffers!" << std::endl;
		errorlogger("FATAL ERROR: Failed to initialize unform buffers");
		exit(EXIT_FAILURE);
	}
	std::cout << "------------ Uniform buffers initialized!\n" << std::endl;

	std::cout << "------------ Initializing framebuffer..." << std::endl;
	if (!init_framebuffers()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to initialize framebuffers in renderer!" << std::endl;
		errorlogger("FATAL ERROR: Failed to initialize framebuffers in renderer!");
		exit(EXIT_FAILURE);
	}
	std::cout << "------------ Framebuffer initialized!\n" << std::endl;

	std::cout << "------------ Initializing shaders..." << std::endl;
	if (!init_shaders(resource_manager)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to initialize shaders in renderer!" << std::endl;
		errorlogger("FATAL ERROR: Failed to initialize shaders in renderer!");
		exit(EXIT_FAILURE);
	}
	std::cout << "------------ Shaders initialized!\n" << std::endl;

	std::cout << "------------ Uploading light data..." << std::endl;
	if (!upload_light_data()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to upload light data in renderer!" << std::endl;
		errorlogger("FATAL ERROR: Failed to upload light data in renderer!");
		exit(EXIT_FAILURE);
	}
	std::cout << "------------ Light data initialized!\n" << std::endl;

	std::cout << "------------ Initializing bloom data..." << std::endl;
	if (!init_bloom_data()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize bloom quad!" << std::endl;
		errorlogger("ERROR: Failed to initialize bloom color quad!");
		exit(EXIT_FAILURE);
	}
	std::cout << "------------ Bloom data initialized!\n" << std::endl;

	/* Set projection matrix */
	update_projection_matrix();
	if (!upload_projection_matrix()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to upload projection matrix!" << std::endl;
		errorlogger("ERROR: Failed to upload projection matrix!");
		exit(EXIT_FAILURE);
	}
}

bool Renderer::init_settings(){
	if (!load_settings()) {
		ortographic = false;
		mouse_visible = true;
		use_vsync = false;
		use_fullscreen = false;
		window_size.x = 1280.0f;
		window_size.y = 640.0f;

		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Failed to load display settings, restoring defaults." << std::endl;
		errorlogger("WARNING: Failed to load display settings, restoring defaults.");

		if (!save_settings()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to save display settings!" << std::endl;
			errorlogger("ERROR: Failed to save display settings!");
			return false;
		}
	}
	SDL_DisableScreenSaver();
	clear();
	return true;
}

bool Renderer::init_window(){
	/* Set to enable opengl window context */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);

	/*Initializes a window to render graphics in*/
	window = SDL_CreateWindow("V8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_size.x, window_size.y, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

	if(use_fullscreen) {
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
	/* Discard all ogl-errors set by glewinit */
	discard_ogl_errors();

	/* Define the viewport dimensions */
	glViewport(0, 0, window_size.x, window_size.y);
	if(check_ogl_error()) {
		std::cout << "ERROR: Failed to Initialize viewport in Display::init_openGL(): " << glewGetErrorString(err) << std::endl;
		errorlogger("ERROR: Failed to Initialize viewport in Display::init_openGL(): ", (const char*)glewGetErrorString(err));
		return false;
	}

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

	return true;
}

bool Renderer::init_uniform_buffers(){
	glGenBuffers(1, &uniform_buffer_matrices);
	  
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_matrices);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, uniform_buffer_matrices, 0, 3 * sizeof(glm::mat4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize matrix uniform buffer in renderer! " << std::endl;
		errorlogger("ERROR: Failed to initialize matrix uniform buffer in renderer!");
		glDeleteBuffers(1, &uniform_buffer_matrices);
		return false;
	}

	uniform_buffers["matrices"] = uniform_buffer_matrices;

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

	if(!bind_g_data(LIGHT_DIRECTIONAL)){
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

	if(!bind_g_data(LIGHT_POINT)){
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

	if(!bind_g_data(LIGHT_SPOT)){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bind g_data in renderer!" << std::endl;
		errorlogger("ERROR: Failed to bind g_data in renderer");
		return false;
	}

	static_geometry_shader = resource_manager.load_shader("geometry_shader_static");
	if (!static_geometry_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load geometry shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load geometry shader in renderer");
		return false;
	}

	static_geometry_shader_colored = resource_manager.load_shader("geometry_shader_static_colored");
	if (!static_geometry_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load geometry shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load geometry shader in renderer");
		return false;
	}

	animated_geometry_shader = resource_manager.load_shader("geometry_shader_animated");
	if (!static_geometry_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load geometry shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load geometry shader in renderer");
		return false;
	}

	animated_geometry_shader_colored = resource_manager.load_shader("geometry_shader_animated_colored");
	if (!static_geometry_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load geometry shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load geometry shader in renderer");
		return false;
	}

	vertical_blur_shader = resource_manager.load_shader("vertical_blur_shader");
	if (!vertical_blur_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load vertical_blur_shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load vertical_blur_shader in renderer");
		return false;
	}

	horizontal_blur_shader = resource_manager.load_shader("horizontal_blur_shader");
	if (!horizontal_blur_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to horizontal_blur_shader shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load horizontal_blur_shader in renderer");
		return false;
	}

	bloom_shader = resource_manager.load_shader("bloom_shader");
	if (!bloom_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bloom_shader shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load bloom_shader in renderer");
		return false;
	}

	return true;
}

bool Renderer::init_framebuffers() {
	if ((window_size.x <= 0) || (window_size.y <= 0)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Display not initialized, cannot initialize framebuffers!" << std::endl;
		errorlogger("ERROR: Display not initialized, cannot initialize framebuffers!");
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
		return false;
	}

	// - Bloom color buffer
	glGenTextures(1, &g_bloom);
	glBindTexture(GL_TEXTURE_2D, g_bloom);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window_size.x, window_size.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, g_bloom, 0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize g_bloom buffer in g_buffer!" << std::endl;
		errorlogger("ERROR: Failed to initialize g_bloom buffer in g_buffer!");
		return false;
	}

	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	glGenRenderbuffers(1, &g_rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, g_rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window_size.x, window_size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, g_rbo_depth);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize g_rbo_depth buffer in g_buffer!" << std::endl;
		errorlogger("ERROR: Failed to initialize g_rbo_depth buffer in g_buffer!");
		return false;
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		print_framebuffer_error_in_fucking_english();
		errorlogger( "ERROR: G-framebuffer not complete!");
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (!set_clear_color_black()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to set clear color!" << std::endl;
		errorlogger("ERROR: Failed to set clear color!");
		return false;
	}

	glGenFramebuffers(2, bb_fbos);
	glGenTextures(2, bb_buffers);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, bb_fbos[i]);
		glBindTexture(GL_TEXTURE_2D, bb_buffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window_size.x, window_size.y, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bb_buffers[i], 0);
		if(check_ogl_error()){
			std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize bloom color buffer in bloom buffer!" << std::endl;
			errorlogger("ERROR: Failed to initialize bloom color buffer in bloom buffer!");
			glDeleteRenderbuffers(1, &g_rbo_depth);
			return false;
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			print_framebuffer_error_in_fucking_english();
			errorlogger( "ERROR: Bloom framebuffers not complete!");
			return false;
		}
	}

	return true;
}

bool Renderer::init_bloom_data(){
	GLfloat quad_vertices[] = {
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	};

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
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

bool Renderer::save_settings(){
	std::cout << "Saving display settings...\n" << std::endl;
	std::ofstream contentf (DISPLAY_SETTINGS_FILE_PATH, std::ios::binary);

	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for display settings!" << std::endl;
		errorlogger("ERROR: Failed to open content file for display settings!");
		return false;
	}

	contentf.write(reinterpret_cast<const char *>(&ortographic), sizeof(GLboolean));
	contentf.write(reinterpret_cast<const char *>(&use_vsync), sizeof(GLboolean));
	contentf.write(reinterpret_cast<const char *>(&use_fullscreen), sizeof(GLboolean));
	contentf.write(reinterpret_cast<const char *>(&mouse_visible), sizeof(GLboolean));

	contentf.write(reinterpret_cast<const char *>(&window_size.x), sizeof(GLfloat));
	contentf.write(reinterpret_cast<const char *>(&window_size.y), sizeof(GLfloat));

	contentf.close();
	std::cout << "Display settings saved!\n" << std::endl;
	return true;
}

bool Renderer::load_settings(){
	std::ifstream contentf (DISPLAY_SETTINGS_FILE_PATH, std::ios::binary);

	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for display settings!" << std::endl;
		errorlogger("ERROR: Failed to open content file for display settings!");
		return false;
	}

	contentf.read(reinterpret_cast<char *>(&ortographic), sizeof(GLboolean));
	contentf.read(reinterpret_cast<char *>(&use_vsync), sizeof(GLboolean));
	contentf.read(reinterpret_cast<char *>(&use_fullscreen), sizeof(GLboolean));
	contentf.read(reinterpret_cast<char *>(&mouse_visible), sizeof(GLboolean));

	contentf.read(reinterpret_cast<char *>(&window_size.x), sizeof(GLfloat));
	contentf.read(reinterpret_cast<char *>(&window_size.y), sizeof(GLfloat));

	contentf.close();

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

bool Renderer::add_context(const Rendering_context_ptr& context) {

	Rendering_context_weak context_weak = context;

	if (context){
		switch (context->shader_type) {
			case GEOMETRY_ANIMATED:
				animated_geom.push_back(context_weak);
				break;
			case GEOMETRY_STATIC:
				static_geom.push_back(context_weak);
				break;
			case GEOMETRY_ANIMATED_COLORED:
				animated_colored_geom.push_back(context_weak);
				break;
			case GEOMETRY_STATIC_COLORED:
				static_colored_geom.push_back(context_weak);
				break;
			case LIGHT_POINT:
				point_lights.push_back(context_weak);
				break;
			case LIGHT_SPOT:
				spot_lights.push_back(context_weak);
				break;
			case LIGHT_DIRECTIONAL:
				dir_lights.push_back(context_weak);
				break;
			case NO_SHADER:
				std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: No shader type set for rendering context!" << std::endl;
				errorlogger("ERROR: No shader type set for rendering context!");
				return false;
			default:
				std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Unknown shader type for rendering context!" << std::endl;
				errorlogger("ERROR: Unknown shader type for rendering context!");
				return false;
		}
		
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Cannot add nullptr to targets!" << std::endl;
		errorlogger("ERROR: Cannot add nullptr to targets!");
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

bool Renderer::render_line(const glm::vec3& start, 
							const glm::vec3& end, 
							const glm::vec3& color){
	return true;
}

//DISPFUNCS

void Renderer::update_projection_matrix(){
	if (ortographic) {
		projection = glm::ortho(0.0f, window_size.x, 0.0f, window_size.y, 0.1f, 6000.0f);
	}
	else{
		projection = glm::perspective(45.0f, window_size.x / window_size.y, 0.1f, 6000.0f);
	}
}

bool Renderer::upload_projection_matrix()const{
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffers.find("matrices")->second);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);  

	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload projection matix!" << std::endl;
		errorlogger("ERROR: Failed to upload projection matix!");
		return false;
	}

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

bool Renderer::enable_fullscreen(){
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	GLint width;
	GLint height;
	SDL_GetWindowSize(window, &width, &height);
	window_size.x = (GLfloat) width;
	window_size.y = (GLfloat) height;
	return true;
}

bool Renderer::set_screen_size(GLuint width, GLuint height){
	SDL_SetWindowSize(window, width, height);
	window_size.x = (GLfloat) width;
	window_size.y = (GLfloat) height;
	update_projection_matrix();
	if (!upload_projection_matrix()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to upload projection matrix!" << std::endl;
		errorlogger("ERROR: Failed to upload projection matrix!");
		return false;
	}
	return true;
}

bool Renderer::update_screen_size(){
	GLint width;
	GLint height;
	SDL_GetWindowSize(window, &width, &height);
	if (width != window_size.x || height != window_size.y) {
		window_size.x = (GLfloat) width;
		window_size.y = (GLfloat) height;
		glViewport(0, 0, window_size.x, window_size.y);
		if(check_ogl_error()) {
			std::cout << "ERROR: Failed to reset viewport!" << std::endl;
			errorlogger("ERROR: Failed to reset viewport!");
			return false;
		}
		delete_g_buffer();
		init_framebuffers();
		if(check_ogl_error()) {
			std::cout << "ERROR: Failed to reset g_buffer!" << std::endl;
			errorlogger("ERROR: Failed to reset g_buffer!");
			return false;
		}

		if (!upload_light_data()) {
			std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to upload light data in renderer!" << std::endl;
			errorlogger("ERROR: Failed to upload light data in renderer!");
			return false;
		}
		update_projection_matrix();
		if (!upload_projection_matrix()) {
			std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to upload projection matrix!" << std::endl;
			errorlogger("ERROR: Failed to upload projection matrix!");
			return false;
		}
	}
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

void Renderer::clear()const{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::present()const{
	SDL_GL_SwapWindow(window);
}

bool Renderer::delete_g_buffer() {
	glDeleteTextures(1, &g_position);
	glDeleteTextures(1, &g_normal);
	glDeleteTextures(1, &g_albedo_spec);
	glDeleteRenderbuffers(1, &g_rbo_depth);
	glDeleteFramebuffers(1, &g_buffer);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed delete g_buffer objects!" << std::endl;
		errorlogger("ERROR: Failed delete g_buffer objects!");
		return false;
	}
	return true;
}

void Renderer::update_view_matrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& camera_up) {
	/*std::cout << "POS: " << position.x << " : " << position.y << " : " << position.z << std::endl;
	std::cout << "TARG: " << target.x << " : " << target.y << " : " << target.z << std::endl;
	std::cout << "CAM: " << camera_up.x << " : " << camera_up.y << " : " << camera_up.z << std::endl;*/
	view = glm::lookAt(position, target, camera_up);
	unrotated_view =  glm::lookAt(position, target, camera_up);
}

bool Renderer::upload_view_matrix()const{
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffers.find("matrices")->second);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view)); 
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(unrotated_view));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload view matrix!" << std::endl;
		errorlogger("ERROR: Failed to upload view matrix!");
		return false;
	}

	return true;
}

/* ================================================================== LightLight */

bool Renderer::bind_g_data(Shader_type light_type)const{
	Shader_ptr current_shader;
	if(light_type == LIGHT_DIRECTIONAL) {
		current_shader = dir_light_shader;
	}

	else if(light_type == LIGHT_POINT) {
		current_shader = point_light_shader;
	}

	else if(light_type == LIGHT_SPOT) {
		current_shader = spot_light_shader;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Invalid light type when binding light shader!" << std::endl;
		errorlogger("ERROR: Invalid light type when binding light shader!");
		return false;
	}

	current_shader->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_position);
	glUniform1i(current_shader->load_uniform_location("g_position"), 0);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind g_position buffer!" << std::endl;
		errorlogger("ERROR: Failed to bind g_position buffer!");
		return false;
	}

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, g_normal);
	glUniform1i(current_shader->load_uniform_location("g_normal"), 1);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind g_normal buffer!" << std::endl;
		errorlogger("ERROR: Failed to bind g_normal buffer!");
		return false;
	}
	
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, g_albedo_spec);
	glUniform1i(current_shader->load_uniform_location("g_albedo_spec"), 2);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind g_albedo_spec buffer!" << std::endl;
		errorlogger("ERROR: Failed to bind g_albedo_spec buffer!");
		return false;
	}
	return true;
}

bool Renderer::upload_view_position(Shader_type shader_type, 
								const glm::vec3& position)const{
	if(shader_type == LIGHT_DIRECTIONAL) {
		glUniform3fv(dir_light_shader->load_uniform_location("view_position"), 1, (float*)&position);
	}

	else if(shader_type == LIGHT_POINT) {
		glUniform3fv(point_light_shader->load_uniform_location("view_position"), 1, (float*)&position);
	}

	else if(shader_type == LIGHT_SPOT) {
		glUniform3fv(spot_light_shader->load_uniform_location("view_position"), 1, (float*)&position);

	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Invalid shader type for light view position update!" << std::endl;
		errorlogger("ERROR: Invalid shader type for light view position update!");
		return false;
	}

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload view position!" << std::endl;
		errorlogger("ERROR: Failed to upload view position!");
		return false;
	}

	return true;
}

bool Renderer::render_lights(const glm::vec3& position){

	if (!bind_g_data(LIGHT_DIRECTIONAL)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind g_data for directional lights!" << std::endl;
		errorlogger("ERROR: Failed to bind g_data for directional lights!");
		return false;
	}
	if (!upload_view_position(LIGHT_DIRECTIONAL, position)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind view position for directional lights!" << std::endl;
		errorlogger("ERROR: Failed to bind view position for directional lights!");
		return false;
	}
	if (!render_dir_lights()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render directional lights!" << std::endl;
		errorlogger("ERROR: Failed to render directional lights!");
		return false;
	}

	if (!bind_g_data(LIGHT_POINT)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind g_data for point lights!" << std::endl;
		errorlogger("ERROR: Failed to bind g_data for point lights!");
		return false;
	}
	if (!upload_view_position(LIGHT_POINT, position)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind view position for point lights!" << std::endl;
		errorlogger("ERROR: Failed to bind view position for point lights!");
		return false;
	}
	if (!render_point_lights()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render point lights!" << std::endl;
		errorlogger("ERROR: Failed to render point lights!");
		return false;
	}

	if (!bind_g_data(LIGHT_SPOT)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind g_data for spot lights!" << std::endl;
		errorlogger("ERROR: Failed to bind g_data for spot lights!");
		return false;
	}
	if (!upload_view_position(LIGHT_SPOT, position)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind view position for spot lights!" << std::endl;
		errorlogger("ERROR: Failed to bind view position for spot lights!");
		return false;
	}
	if (!render_spot_lights()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render spot lights!" << std::endl;
		errorlogger("ERROR: Failed to render spot lights!");
		return false;
	}
	return true;
}

bool Renderer::render_dir_lights(){
	for (auto light_context = dir_lights.begin(); light_context != dir_lights.end(); ++light_context) {
		auto context = light_context->lock();
		if (!context) {
			SDL_Log("Directional light context expired, removing from renderer...");
			dir_lights.erase(light_context);
			continue;
		}
		else if (!render_dir_light(context, dir_light_shader)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render directional light!" << std::endl;
			errorlogger("ERROR: Failed to render directional light!");
			return false;
		}
	}
	return true;
}

bool Renderer::render_point_lights(){
	for (auto light_context = point_lights.begin(); light_context != point_lights.end(); ++light_context) {
		auto context = light_context->lock();
		if (!context) {
			SDL_Log("Point light context expired, removing from renderer...");
			//point_lights.erase(light_context);
			continue;
		}
		else if (!render_light(context, point_light_shader)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render point light!" << std::endl;
			errorlogger("ERROR: Failed to render point light!");
			return false;
		}
	}
	return true;
}

bool Renderer::render_spot_lights(){
	for (auto light_context = spot_lights.begin(); light_context != spot_lights.end(); ++light_context) {
		auto context = light_context->lock();
		if (!context) {
			SDL_Log("Spot light context expired, removing from renderer...");
			//spot_lights.erase(light_context);
			continue;
		}
		else if (!render_light(context, spot_light_shader)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render spot light!" << std::endl;
			errorlogger("ERROR: Failed to render spot light!");
			return false;
		}
	}
	return true;
}

bool Renderer::render_dir_light(const Rendering_context_ptr& context, const Shader_ptr& shader)const{
	if (!context->setup_base_uniforms(shader)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to setup base uniforms for light" << std::endl;
		errorlogger("ERROR: Failed to setup base uniforms for light!");
		return false;
	}

	glBindVertexArray(context->VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render light quad for directional light!" << std::endl;
		errorlogger("ERROR: Failed to render light quad for directional light!");
		return false;
	}
	return true;
}

bool Renderer::render_light(const Rendering_context_ptr& context, const Shader_ptr& shader)const{

	if (!context->setup_base_uniforms(shader)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to setup base uniforms for light" << std::endl;
		errorlogger("ERROR: Failed to setup base uniforms for light!");
		return false;
	}
	
	glPolygonMode(GL_FRONT_AND_BACK, context->render_mode);
	glBindVertexArray(context->VAO);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW); 
	if (context->render_elements) {
		glDrawElements(GL_TRIANGLES, context->num_vertices, GL_UNSIGNED_INT, 0);
	}
	else{
		glDrawArrays(GL_TRIANGLES, 0, context->num_vertices);
	}
	glDisable(GL_CULL_FACE);
	glBindVertexArray(0);
	
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render light!" << std::endl;
		errorlogger("ERROR: Failed to render light!");
		return false;
	}

	return true;
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

/* ================================================================== GeomGeom */

bool Renderer::setup_geometry_rendering(const Camera_ptr& camera){
	update_view_matrix(camera->get_position_refrence(), 
						camera->get_target_refrence(), 
						camera->get_up_dir_refrence());
	upload_view_matrix();
	use_g_buffer();
	glDepthMask(GL_TRUE);
	clear();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to setup geometry rendering!" << std::endl;
		errorlogger("ERROR: Failed to setup geometry rendering!");
		return false;
	}

	return true;
}

bool Renderer::render_geometry(const Camera_ptr& camera){
	update_screen_size();
	if (!setup_geometry_rendering(camera)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to setup geometry rendering!" << std::endl;
		errorlogger("ERROR: Failed to setup geometry rendering!");
		return false;
	}
		
	static_geometry_shader->use();
	if (!render_static_geomety()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render static geometry!" << std::endl;
		errorlogger("ERROR: Failed to render static geometry!");
		return false;
	}

	static_geometry_shader_colored->use();
	if (!render_static_colored_geomety()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render static colored geometry!" << std::endl;
		errorlogger("ERROR: Failed to render static colored geometry!");
		return false;
	}

	animated_geometry_shader->use();
	if (!render_animated_geomety()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render animated geometry!" << std::endl;
		errorlogger("ERROR: Failed to render animated geometry!");
		return false;
	}

	animated_geometry_shader_colored->use();
	if (!render_animated_colored_geomety()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render animated colored geometry!" << std::endl;
		errorlogger("ERROR: Failed to render animated colored geometry!");
		return false;
	}

	if (!bloom_pass(10)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render bloom!" << std::endl;
		errorlogger("ERROR: Failed to render bloom!");
		return false;
	}
	
	if (!detach_geometry_rendering()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to detach geometry rendering!" << std::endl;
		errorlogger("ERROR: Failed to detach geometry rendering!");
		return false;
	}

	return true;
}

bool Renderer::detach_geometry_rendering()const{
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
		return false;
	}

	return true;
}

bool Renderer::render_static_geomety()const{
	for (auto static_context : static_geom) {
		auto context = static_context.lock();
		if (!context) {
			SDL_Log("Static geometry context expired, removing from renderer...");
			/* TODO::Remove context */
			continue;
		}
		else if (!render_base_geometry(context, static_geometry_shader)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render static geometry!" << std::endl;
			errorlogger("ERROR: Failed to render static geometry!");
			return false;
		}
	}
	return true;
}

bool Renderer::render_static_colored_geomety()const{
	for (auto static_colored_context : static_colored_geom) {
		auto context = static_colored_context.lock();
		if (!context) {
			SDL_Log("Static colored geometry context expired, removing from renderer...");
			/* TODO::Remove context */
			continue;
		}
		else if (!render_base_geometry(context, static_geometry_shader_colored)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render static colored geometry!" << std::endl;
			errorlogger("ERROR: Failed to render static colored geometry!");
			return false;
		}
	}
	return true;
}

bool Renderer::render_animated_geomety()const{
	for (auto anim_context : animated_geom) {
		auto context = anim_context.lock();
		if (!context) {
			SDL_Log("Animated geometry context expired, removing from renderer...");
			/* TODO::Remove context */
			continue;
		}
		else if (!render_base_geometry(context, animated_geometry_shader)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render animated geometry!" << std::endl;
			errorlogger("ERROR: Failed to render animated geometry!");
			return false;
		}
	}
	return true;
}

bool Renderer::render_animated_colored_geomety()const{
	for (auto anim_colored_context : animated_colored_geom) {
		auto context = anim_colored_context.lock();
		if (!context) {
			SDL_Log("Animated colored geometry context expired, removing from renderer...");
			/* TODO::Remove context */
			continue;
		}
		else if (!render_base_geometry(context, animated_geometry_shader_colored)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render animated colored geometry!" << std::endl;
			errorlogger("ERROR: Failed to render animated colored geometry!");
			return false;
		}
	}
	return true;
}


bool Renderer::render_base_geometry(const Rendering_context_ptr& context, 
								const Shader_ptr& shader)const{

	context->setup_base_uniforms(shader);
	
	for (auto instance_setup : context->instance_uniform_setups) {
		if (!instance_setup.second(shader)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to setup instance uniforms!" << std::endl;
			errorlogger("ERROR: Failed to setup instance uniforms!");
			return false;
		}
		else{
			glPolygonMode(GL_FRONT_AND_BACK, context->render_mode);
			glBindVertexArray(context->VAO);
			if (context->render_elements) {
				glDrawElements(GL_TRIANGLES, context->num_vertices, GL_UNSIGNED_INT, 0);
			}
			else{
				glDrawArrays(GL_TRIANGLES, 0, context->num_vertices);
			}
			glBindVertexArray(0);

			if(check_ogl_error()){
				std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render elements!" << std::endl;
				errorlogger("ERROR: Failed to render elements!");
				return false;
			}
		}
		
	}
   
	return true;
}

/* ================================================================== BloomBloom */

bool Renderer::bloom_pass(GLuint amount)const{
	if ((amount % 2) != 0) {
		++amount;
	}

	GLboolean first_shader_set = false;
	GLboolean second_shader_set = false;

	for (GLuint i = 0; i < amount; i++) {
		if (!first_shader_set && (i < amount/2)) {
			first_shader_set = true;
			vertical_blur_shader->use();
		}
		else if (!second_shader_set && (i >= amount/2)) {
			second_shader_set = true;
			horizontal_blur_shader->use();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, bb_fbos[1 - (i % 2)]); 
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(bloom_shader->load_uniform_location("bloom"), 0);
		glBindTexture(GL_TEXTURE_2D, (i == 0) ? g_bloom : bb_buffers[i % 2]); 
		if (!render_bloom_quad()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render light quad for bloom calculations!" << std::endl;
			errorlogger("ERROR: Failed to render light quad for bloom calculations!");
			return false;
		}
	}

	return true;
}

bool Renderer::render_bloom_quad()const{
	glBindVertexArray(quad_VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind light quad for bloom calculations!" << std::endl;
		errorlogger("ERROR: Failed to bind light quad for bloom calculations!");
		return false;
	}
	return true;
}

bool Renderer::render_bloom()const{
	bloom_shader->use();
	glActiveTexture(GL_TEXTURE0 + 5);
	glUniform1i(bloom_shader->load_uniform_location("bloom"), 5);
	glBindTexture(GL_TEXTURE_2D, bb_buffers[0]); 
	
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind bloom buffer!" << std::endl;
		errorlogger("ERROR: Failed to bind bloom buffer!");
		return false;
	}

	if (!render_bloom_quad()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render bloom quad!" << std::endl;
		errorlogger("ERROR: Failed to render bloom quad!");
		return false;
	}

	return true;
}