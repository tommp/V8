#include "renderer.h"

Renderer::Renderer(){
};

Renderer::~Renderer() {
	if (!delete_buffers()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Failed to delete buffers!" << std::endl;
		errorlogger("WARNING: Failed to delete buffers!");
	}
	if (!delete_uniform_buffers()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Failed to delete uniform buffers!" << std::endl;
		errorlogger("WARNING: Failed to delete uniform buffers!");
	}

	SDL_GL_DeleteContext(gl_context); 
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

	std::cout << "------------ Initializing primitives..." << std::endl;
	if (!init_primitives(resource_manager)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to initialize primitives!" << std::endl;
		errorlogger("FATAL ERROR: Failed to initialize primitives!");
		exit(EXIT_FAILURE);
	}
	std::cout << "------------ Primitives initialized!\n" << std::endl;

	std::cout << "------------ Uploading initial uniform data..." << std::endl;
	if (!init_upload_uniform_data()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "FATAL ERROR: Failed to initialize uniform data!" << std::endl;
		errorlogger("FATAL ERROR: Failed to initialize uniform data!");
		exit(EXIT_FAILURE);
	}
	std::cout << "------------ Initial uniform data uploaded!\n" << std::endl;
}

bool Renderer::init_upload_uniform_data(){

	if (!settings_initialized){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Settings not initialized, cannot initialize window!"<<std::endl;
		errorlogger("ERROR: Settings not initialized, cannot initialize window!");
		return false;
	}

	std::cout << "Uploading settings..." << std::endl;
	if (!upload_settings()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to upload settings in renderer!" << std::endl;
		errorlogger("ERROR: Failed to upload settings in renderer!");
		return false;
	}
	std::cout << "Settings uploaded!\n" << std::endl;

	std::cout << "Uploading resolution data..." << std::endl;
	if (!upload_res_data()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to upload resolution data in renderer!" << std::endl;
		errorlogger("ERROR: Failed to upload resolution data in renderer!");
		return false;
	}
	std::cout << "Resolution data uploaded!\n" << std::endl;

	std::cout << "Updating projection matrix..." << std::endl;
	if (!update_projection_matrix()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to update projection matrix!" << std::endl;
		errorlogger("ERROR: Failed to update projection matrix!");
		return false;
	}
	std::cout << "Projection matrix updated!\n" << std::endl;

	std::cout << "Uploading projection matrix..." << std::endl;
	if (!upload_projection_matrix()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to upload projection matrix!" << std::endl;
		errorlogger("ERROR: Failed to upload projection matrix!");
		return false;
	}
	std::cout << "Projection matrix uploaded!\n" << std::endl;

	uniform_data_initialized = true;

	return true;
}

bool Renderer::init_settings(){
	if (!load_settings()) {
		/* These are the default settings */
		ortographic = false;
		use_mouse = true;
		
		window_size.x = 640.0f * 2;
		window_size.y = 320.0f * 2;
		resolution.x = 640.0f * 2;
		resolution.y = 320.0f * 2;

		use_vsync = true;
		use_fullscreen = false;
		use_AA = true;
		use_SSAO = true;
		use_pre_bloom = true;
		use_post_bloom = true;
		use_shadows = false;

		near_plane = 10.0;
		far_plane = 3000.0;

		lower_bloom_threshold = 0.6;
		higher_bloom_threshold = 1.0;

		gamma = 2.2;
		exposure = 1.0;

		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Failed to load display settings, restoring defaults." << std::endl;
		errorlogger("WARNING: Failed to load display settings, restoring defaults.");

		if (!save_settings()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to save display settings!" << std::endl;
			errorlogger("ERROR: Failed to save display settings!");
			return false;
		}
	}

	SDL_DisableScreenSaver();

	settings_initialized = true;

	return true;
}

bool Renderer::init_window(){

	if (!settings_initialized){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Settings not initialized, cannot initialize window!"<<std::endl;
		errorlogger("ERROR: Settings not initialized, cannot initialize window!");
		return false;
	}

	window_init_pos = glm::vec2(0.0, 0.0);

	/* Set to enable opengl window context */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, Renderer_consts::OPENGL_MAJOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, Renderer_consts::OPENGL_MINOR_VERSION);

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

	if (!use_mouse) {
		SDL_ShowCursor(0);
	}
	else{
		SDL_ShowCursor(1);
	}

	window_initialized = true;

	return true;
}

bool Renderer::init_openGL(){

	if (!window_initialized){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Window not initialized, cannot initialize OpenGL!"<<std::endl;
		errorlogger("ERROR: Window not initialized, cannot initialize OpenGL!");
		return false;
	}

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

	glViewport(0, 0, resolution.x, resolution.y);
	if(check_ogl_error()) {
		std::cout << "ERROR: Failed to Initialize viewport: " << glewGetErrorString(err) << std::endl;
		errorlogger("ERROR: Failed to Initialize viewport: ", (const char*)glewGetErrorString(err));
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to Initialize depth testing in Display::init_openGL()!" << std::endl;
		errorlogger("ERROR: Failed to Initialize depth testing in Display::init_openGL()!");
		return false;
	}

	glClearColor(Renderer_consts::CLEARCOLOR[0], 
				Renderer_consts::CLEARCOLOR[1], 
				Renderer_consts::CLEARCOLOR[2], 
				Renderer_consts::CLEARCOLOR[3]);

	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to Initialize clearcolour in Display::init_openGL()!" << std::endl;
		errorlogger("ERROR: Failed to Initialize clearcolour in Display::init_openGL()!");
		return false;
	}

	ogl_initialized = true;

	return true;
}

bool Renderer::init_uniform_buffers(){

	if (!ogl_initialized){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: OpenGL not initialized, cannot initialize uniform buffers!"<<std::endl;
		errorlogger("ERROR: OpenGL not initialized, cannot initialize uniform buffers!");
		return false;
	}

	GLuint uniform_buffer_matrices;
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

	GLuint uniform_buffer_res_data;
	glGenBuffers(1, &uniform_buffer_res_data);
	  
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_res_data);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec2), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 2, uniform_buffer_res_data, 0, 2 * sizeof(glm::vec2));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize resolution data uniform buffer in renderer! " << std::endl;
		errorlogger("ERROR: Failed to initialize resolution data uniform buffer in renderer!");
		glDeleteBuffers(1, &uniform_buffer_res_data);
		return false;
	}

	uniform_buffers["resolution_data"] = uniform_buffer_res_data;

	GLuint uniform_buffer_plane_data;
	glGenBuffers(1, &uniform_buffer_plane_data);
	  
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_plane_data);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 3, uniform_buffer_plane_data, 0, sizeof(glm::vec2));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize plane data uniform buffer in renderer! " << std::endl;
		errorlogger("ERROR: Failed to initialize plane data uniform buffer in renderer!");
		glDeleteBuffers(1, &uniform_buffer_plane_data);
		return false;
	}

	uniform_buffers["plane_data"] = uniform_buffer_plane_data;

	GLuint uniform_buffer_settings;
	glGenBuffers(1, &uniform_buffer_settings);
	  
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_settings);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 4, uniform_buffer_settings, 0, 2 * sizeof(glm::vec4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize plane data uniform buffer in renderer! " << std::endl;
		errorlogger("ERROR: Failed to initialize plane data uniform buffer in renderer!");
		glDeleteBuffers(1, &uniform_buffer_settings);
		return false;
	}

	uniform_buffers["settings"] = uniform_buffer_settings;


	GLuint uniform_buffer_camera_data;
	glGenBuffers(1, &uniform_buffer_camera_data);

	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_camera_data);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 5, uniform_buffer_camera_data, 0, sizeof(glm::vec3));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize camera data uniform buffer in renderer! " << std::endl;
		errorlogger("ERROR: Failed to initialize camera data uniform buffer in renderer!");
		glDeleteBuffers(1, &uniform_buffer_camera_data);
		return false;
	}

	uniform_buffers["camera_data"] = uniform_buffer_camera_data;


	GLuint uniform_buffer_directional_lights;
	glGenBuffers(1, &uniform_buffer_directional_lights);
	GLuint dir_light_buffer_size = Utility_consts::SIZEOF_DIR_LIGHT * Utility_consts::MAX_DIR_LIGHTS;

	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_directional_lights);
	glBufferData(GL_UNIFORM_BUFFER, dir_light_buffer_size, NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 6, uniform_buffer_directional_lights, 0, dir_light_buffer_size);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize directional light uniform buffer in renderer! " << std::endl;
		errorlogger("ERROR: Failed to initialize directional light uniform buffer in renderer!");
		glDeleteBuffers(1, &uniform_buffer_directional_lights);
		return false;
	}

	uniform_buffers["dir_lights"] = uniform_buffer_directional_lights;


	GLuint uniform_buffer_point_lights;
	glGenBuffers(1, &uniform_buffer_point_lights);
	GLuint point_light_buffer_size = Utility_consts::SIZEOF_POINT_LIGHT * Utility_consts::MAX_POINT_LIGHTS + sizeof(GLint);

	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_point_lights);
	glBufferData(GL_UNIFORM_BUFFER, point_light_buffer_size, NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 7, uniform_buffer_point_lights, 0, point_light_buffer_size);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize directional light uniform buffer in renderer! " << std::endl;
		errorlogger("ERROR: Failed to initialize directional light uniform buffer in renderer!");
		glDeleteBuffers(1, &uniform_buffer_point_lights);
		return false;
	}

	uniform_buffers["point_lights"] = uniform_buffer_point_lights;


	GLuint uniform_buffer_spot_lights;
	glGenBuffers(1, &uniform_buffer_spot_lights);
	GLuint spot_light_buffer_size = Utility_consts::SIZEOF_SPOT_LIGHT * Utility_consts::MAX_SPOT_LIGHTS  + sizeof(GLint);

	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_spot_lights);
	glBufferData(GL_UNIFORM_BUFFER, spot_light_buffer_size, NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 8, uniform_buffer_spot_lights, 0, spot_light_buffer_size);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize directional light uniform buffer in renderer! " << std::endl;
		errorlogger("ERROR: Failed to initialize directional light uniform buffer in renderer!");
		glDeleteBuffers(1, &uniform_buffer_spot_lights);
		return false;
	}

	uniform_buffers["spot_lights"] = uniform_buffer_spot_lights;


	uniform_buffers_initialized = true;

	return true;
}

bool Renderer::init_shaders(Resource_manager& resource_manager){
	if (!ogl_initialized){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: OpenGL not initialized, cannot initialize shaders!"<<std::endl;
		errorlogger("ERROR: OpenGL not initialized, cannot initialize shaders!");
		return false;
	}

	dir_light_shader = resource_manager.load_shader("dir_light_shader");
	if (dir_light_shader == nullptr) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load directional light shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load directional light shader in renderer");
		return false;
	}

	point_light_shader = resource_manager.load_shader("point_light_shader");
	if (point_light_shader == nullptr) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load point light shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load point light shader in renderer");
		return false;
	}

	spot_light_shader = resource_manager.load_shader("spot_light_shader");
	if (spot_light_shader == nullptr) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load spot light shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load spot light shader in renderer");
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

	primitive_line_shader = resource_manager.load_shader("primitive_line_shader");
	if (!primitive_line_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load primitive line shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load primitive line shader in renderer");
		return false;
	}

	bloom_shader = resource_manager.load_shader("bloom_shader");
	if (!bloom_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load vbloom_shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load bloom_shader in renderer");
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
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load horizontal_blur_shader shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load horizontal_blur_shader in renderer");
		return false;
	}

	ppe_blend_shader = resource_manager.load_shader("ppe_blend_shader");
	if (!ppe_blend_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load ppe_blend_shader shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load ppe_blend_shader in renderer");
		return false;
	}

	HDR_shader = resource_manager.load_shader("HDR_shader");
	if (!HDR_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load HDR_shader shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load HDR_shader in renderer");
		return false;
	}

	FXAA_shader = resource_manager.load_shader("FXAA_shader");
	if (!FXAA_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to FXAA_shader shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load FXAA_shader in renderer");
		return false;
	}

	SSAO_shader = resource_manager.load_shader("SSAO_shader");
	if (!SSAO_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to SSAO_shader shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load SSAO_shader in renderer");
		return false;
	}

	LFST_cull_shader = resource_manager.load_shader("LFST_cull_shader");
	if (!LFST_cull_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to LFST_cull_shader shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load LFST_cull_shader in renderer");
		return false;
	}

	LFST_layer_shader = resource_manager.load_shader("LFST_layer_shader");
	if (!LFST_layer_shader) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to LFST_layer_shader shader in renderer!" << std::endl;
		errorlogger("ERROR: Failed to load LFST_layer_shader in renderer");
		return false;
	}

	shaders_initialized = true;

	return true;
}

bool Renderer::init_g_buffer(){
	glGenFramebuffers(1, &g_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
	  
	glGenTextures(1, &g_position);
	glBindTexture(GL_TEXTURE_2D, g_position);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, resolution.x, resolution.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_position, 0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize g_position buffer in g_buffer!" << std::endl;
		errorlogger("ERROR: Failed to initialize g_position buffer in g_buffer!");
		return false;
	}

	glGenTextures(1, &g_normal);
	glBindTexture(GL_TEXTURE_2D, g_normal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, resolution.x, resolution.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_normal, 0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize g_normal buffer in g_buffer!" << std::endl;
		errorlogger("ERROR: Failed to initialize g_normal buffer in g_buffer!");
		return false;
	}

	glGenTextures(1, &g_albedo_spec);
	glBindTexture(GL_TEXTURE_2D, g_albedo_spec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, resolution.x, resolution.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_albedo_spec, 0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize g_albedo_spec buffer in g_buffer!" << std::endl;
		errorlogger("ERROR: Failed to initialize g_albedo_spec buffer in g_buffer!");
		return false;
	}

	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, attachments);

	glGenRenderbuffers(1, &g_rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, g_rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, resolution.x, resolution.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, g_rbo_depth);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize g_rbo_depth buffer in g_buffer!" << std::endl;
		errorlogger("ERROR: Failed to initialize g_rbo_depth buffer in g_buffer!");
		return false;
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		print_framebuffer_error_in_english();
		errorlogger( "ERROR: G-framebuffer not complete!");
		return false;
	}

	framebuffers_initialized = true;

	return true;
}

bool Renderer::init_blur_buffers(){
	glGenFramebuffers(2, bb_fbos);
	glGenTextures(2, bb_buffers);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, bb_fbos[i]);
		glBindTexture(GL_TEXTURE_2D, bb_buffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, resolution.x, resolution.y, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bb_buffers[i], 0);
		if(check_ogl_error()){
			std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize blur color buffer in blur fbo!" << std::endl;
			errorlogger("ERROR: Failed to initialize blur color buffer in blur fbo!");
			glDeleteRenderbuffers(1, &g_rbo_depth);
			return false;
		}

		GLuint bb_attachment = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &bb_attachment);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			print_framebuffer_error_in_english();
			errorlogger( "ERROR: Blur framebuffers not complete!");
			return false;
		}
	}

	return true;
}

bool Renderer::init_AA_buffer(){
	glGenFramebuffers(1, &AA_fbo);
	glGenTextures(1, &AA_buffer);

	glBindFramebuffer(GL_FRAMEBUFFER, AA_fbo);
	glBindTexture(GL_TEXTURE_2D, AA_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, resolution.x, resolution.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, AA_buffer, 0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize color buffer in AA_fbo!" << std::endl;
		errorlogger("ERROR: Failed to initialize color buffer in AA_fbo!");
		return false;
	}

	GLuint aa_attachment = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &aa_attachment);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		print_framebuffer_error_in_english();
		errorlogger( "ERROR: AA framebuffer not complete!");
		return false;
	}

	return true;
}

bool Renderer::init_SSAO_buffer(){
	glGenFramebuffers(1, &SSAO_fbo);
	glGenTextures(1, &SSAO_buffer);

	glBindFramebuffer(GL_FRAMEBUFFER, SSAO_fbo);
	glBindTexture(GL_TEXTURE_2D, SSAO_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, resolution.x, resolution.y, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SSAO_buffer, 0); 
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize SSAO_buffer in SSAO_fbo!" << std::endl;
		errorlogger("ERROR: Failed to initialize SSAO_buffer in SSAO_fbo!");
		return false;
	}

	GLuint SSAO_attachment = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &SSAO_attachment);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		print_framebuffer_error_in_english();
		errorlogger( "ERROR: SSAO framebuffer not complete!");
		return false;
	}

	return true;
}

bool Renderer::init_light_buffer(){
	glGenFramebuffers(1, &light_fbo);
	glGenTextures(1, &light_color_buffer);

	glBindFramebuffer(GL_FRAMEBUFFER, light_fbo);
	glBindTexture(GL_TEXTURE_2D, light_color_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, resolution.x, resolution.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, light_color_buffer, 0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize light_color_buffer in light_fbo!" << std::endl;
		errorlogger("ERROR: Failed to initialize light_color_buffer in light_fbo!");
		return false;
	}

	GLuint light_attachment = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &light_attachment);

	glGenRenderbuffers(1, &light_rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, light_rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, resolution.x, resolution.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, light_rbo_depth);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize g_rbo_depth buffer in g_buffer!" << std::endl;
		errorlogger("ERROR: Failed to initialize g_rbo_depth buffer in g_buffer!");
		return false;
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		print_framebuffer_error_in_english();
		errorlogger( "ERROR: Light framebuffer not complete!");
		return false;
	}

	return true;
}

bool Renderer::init_shadow_buffers(){
	for (GLuint i = 0; i < Renderer_consts::SHADOW_LAYERS; ++i) {
		glGenFramebuffers(1, &shadow_layer_fbos[i]);
	    glGenTextures(1, &shadow_layers[i]);
	    glBindTexture(GL_TEXTURE_2D, shadow_layers[i]);

	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, resolution.x, resolution.y, 0, GL_RG, GL_FLOAT, NULL);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	    glBindFramebuffer(GL_FRAMEBUFFER, shadow_layer_fbos[i]);
	    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadow_layers[i], 0);

	    GLuint shadow_attachment = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &shadow_attachment);

	    if(check_ogl_error()){
			std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize shadow layer buffer!" << std::endl;
			errorlogger("ERROR: Failed to initialize shadow layer buffer!");
			return false;
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			print_framebuffer_error_in_english();
			errorlogger( "ERROR: Shadow layer framebuffer not complete!");
			return false;
		}

		glGenFramebuffers(1, &shadow_front_cull_fbos[i]);
	    glGenTextures(1, &shadow_front_cull_buffers[i]);
	    glBindTexture(GL_TEXTURE_2D, shadow_front_cull_buffers[i]);

	    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, resolution.x, resolution.y, 0, GL_RED, GL_FLOAT, NULL);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	    glBindFramebuffer(GL_FRAMEBUFFER, shadow_front_cull_fbos[i]);
	    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadow_front_cull_buffers[i], 0);

	    shadow_attachment = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &shadow_attachment);

		glGenRenderbuffers(1, &shadow_front_cull_depth[i]);
		glBindRenderbuffer(GL_RENDERBUFFER, shadow_front_cull_depth[i]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, resolution.x, resolution.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, shadow_front_cull_depth[i]);

	    if(check_ogl_error()){
			std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize shadow front cull buffer!" << std::endl;
			errorlogger("ERROR: Failed to initialize shadow front cull buffer!");
			return false;
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			print_framebuffer_error_in_english();
			errorlogger( "ERROR: Shadow front cull framebuffer not complete!");
			return false;
		}

		glGenFramebuffers(1, &shadow_back_cull_fbos[i]);
	    glGenTextures(1, &shadow_back_cull_buffers[i]);
	    glBindTexture(GL_TEXTURE_2D, shadow_back_cull_buffers[i]);

	    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, resolution.x, resolution.y, 0, GL_RED, GL_FLOAT, NULL);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	    glBindFramebuffer(GL_FRAMEBUFFER, shadow_back_cull_fbos[i]);
	    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadow_back_cull_buffers[i], 0);

	    shadow_attachment = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &shadow_attachment);

		glGenRenderbuffers(1, &shadow_back_cull_depth[i]);
		glBindRenderbuffer(GL_RENDERBUFFER, shadow_back_cull_depth[i]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, resolution.x, resolution.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, shadow_back_cull_depth[i]);


	    if(check_ogl_error()){
			std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize shadow back cull buffer!" << std::endl;
			errorlogger("ERROR: Failed to initialize shadow back cull buffer!");
			return false;
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			print_framebuffer_error_in_english();
			errorlogger( "ERROR: Shadow back cull framebuffer not complete!");
			return false;
		}
	}

	return true;
}

bool Renderer::init_bloom_buffers(){
	glGenFramebuffers(1, &pre_bloom_fbo);
	glGenTextures(1, &pre_bloom_buffer);

	glBindFramebuffer(GL_FRAMEBUFFER, pre_bloom_fbo);
	glBindTexture(GL_TEXTURE_2D, pre_bloom_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, resolution.x, resolution.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pre_bloom_buffer, 0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize color buffer in pre_bloom_fbo!" << std::endl;
		errorlogger("ERROR: Failed to initialize color buffer in pre_bloom_fbo!");
		return false;
	}

	GLuint pre_bloom_attachment = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &pre_bloom_attachment);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		print_framebuffer_error_in_english();
		errorlogger( "ERROR: Pre bloom framebuffer not complete!");
		return false;
	}

	glGenFramebuffers(1, &post_bloom_fbo);
	glGenTextures(1, &post_bloom_buffer);

	glBindFramebuffer(GL_FRAMEBUFFER, post_bloom_fbo);
	glBindTexture(GL_TEXTURE_2D, post_bloom_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, resolution.x, resolution.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, post_bloom_buffer, 0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize color buffer in post_bloom_fbo!" << std::endl;
		errorlogger("ERROR: Failed to initialize color buffer in post_bloom_fbo!");
		return false;
	}

	GLuint post_bloom_attachment = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &post_bloom_attachment);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		print_framebuffer_error_in_english();
		errorlogger( "ERROR: Post bloom framebuffer not complete!");
		return false;
	}

	return true;
}

bool Renderer::init_ppe_blend_buffer(){
	glGenFramebuffers(1, &ppe_blend_fbo);
	glGenTextures(1, &ppe_blend_buffer);

	glBindFramebuffer(GL_FRAMEBUFFER, ppe_blend_fbo);
	glBindTexture(GL_TEXTURE_2D, ppe_blend_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, resolution.x, resolution.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ppe_blend_buffer, 0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize color buffer in ppe_blend_fbo!" << std::endl;
		errorlogger("ERROR: Failed to initialize color buffer in ppe_blend_fbo!");
		return false;
	}

	GLuint ppe_blend_attachment = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &ppe_blend_attachment);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		print_framebuffer_error_in_english();
		errorlogger( "ERROR: PPE blend framebuffer not complete!");
		return false;
	}

	return true;
}

bool Renderer::init_framebuffers() {

	if (!ogl_initialized){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: OpenGL not initialized, cannot initialize framebuffers!"<<std::endl;
		errorlogger("ERROR: OpenGL not initialized, cannot initialize framebuffers!");
		return false;
	}

	if ((window_size.x <= 0) || (window_size.y <= 0)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Display not initialized, cannot initialize framebuffers!" << std::endl;
		errorlogger("ERROR: Display not initialized, cannot initialize framebuffers!");
		return false;
	}

	if (!init_g_buffer()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: failed to initialize g_buffer!" << std::endl;
		errorlogger("ERROR: Failed to initialize g_buffer!");
		return false;
	}

	if (!init_blur_buffers()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: failed to initialize blur buffers!" << std::endl;
		errorlogger("ERROR: Failed to initialize blur buffers!");
		return false;
	}

	if (!init_bloom_buffers()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: failed to initialize bloom buffers!" << std::endl;
		errorlogger("ERROR: Failed to initialize bloom buffers!");
		return false;
	}

	if (!init_AA_buffer()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: failed to initialize AA buffer!" << std::endl;
		errorlogger("ERROR: Failed to initialize AA buffer!");
		return false;
	}

	if (!init_SSAO_buffer()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: failed to initialize SSAO buffer!" << std::endl;
		errorlogger("ERROR: Failed to initialize SSAO buffer!");
		return false;
	}

	if (!init_light_buffer()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: failed to initialize light buffer!" << std::endl;
		errorlogger("ERROR: Failed to initialize light buffer!");
		return false;
	}

	if (!init_shadow_buffers()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: failed to initialize shadow buffers!" << std::endl;
		errorlogger("ERROR: Failed to initialize shadow buffers!");
		return false;
	}

	if(!init_ppe_blend_buffer()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: failed to initialize ppe blend buffers!" << std::endl;
		errorlogger("ERROR: Failed to initialize ppe blend buffers!");
		return false;
	}

	framebuffers_initialized = true;

	return true;
}

bool Renderer::init_quad(){
	GLfloat quad_vertices[] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
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

bool Renderer::init_cube(){
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

	glGenVertexArrays(1, &cube_VAO);
	glGenBuffers(1, &cube_VBO);
	glGenBuffers(1, &cube_EBO);

	glBindVertexArray(cube_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_EBO);

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bind cube VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to cube VAO in renderer!");
		glDeleteBuffers(1, &cube_VBO);
		glDeleteBuffers(1, &cube_EBO);
		glDeleteVertexArrays(1, &cube_VAO);
		return false;
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), &quad_indices, GL_STATIC_DRAW);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to buffer vertex data for cube VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to buffer vertex data for cube VAO in renderer!");
		glDeleteBuffers(1, &cube_VBO);
		glDeleteBuffers(1, &cube_EBO);
		glDeleteVertexArrays(1, &cube_VAO);
		return false;
	}
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to set vertex attributes for cube VAO in renderer!" << std::endl;
		errorlogger("ERROR: Failed to set vertex attributes for cube VAO in renderer!");
		glDeleteBuffers(1, &cube_VBO);
		glDeleteBuffers(1, &cube_EBO);
		glDeleteVertexArrays(1, &cube_VAO);
		return false;
	}
	return true;
}

bool Renderer::init_primitives(Resource_manager& resource_manager){
	(void(resource_manager));

	if (!ogl_initialized){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: OpenGL not initialized, cannot initialize primitives!"<<std::endl;
		errorlogger("ERROR: OpenGL not initialized, cannot initialize primitives!");
		return false;
	}

#if ENABLE_BULLET_DEBUG
	if (!(line = resource_manager.load_mesh("LINE"))) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to load line mesh primitive!" << std::endl;
		errorlogger("ERROR: Failed to load line mesh primitive!");
		return false;
	}
#endif

	if (!init_quad()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize quad!" << std::endl;
		errorlogger("ERROR: Failed to initialize quad!");
		return false;
	}

	if (!init_cube()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to initialize cube!" << std::endl;
		errorlogger("ERROR: Failed to initialize cube!");
		return false;
	}

	primitives_initialized = true;

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

	if (use_SSAO) {
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, SSAO_buffer);
		glUniform1i(current_shader->load_uniform_location("SSAO_buffer"), 3);
		if(check_ogl_error()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind SSAO_buffer buffer!" << std::endl;
			errorlogger("ERROR: Failed to bind SSAO_buffer buffer!");
			return false;
		}
	}

	if (use_shadows){
		for (GLuint i = 0; i < Renderer_consts::SHADOW_LAYERS; ++i) {
			glActiveTexture(GL_TEXTURE4 + i);
			glUniform1i(current_shader->load_uniform_location("shadow_layers", i), 4 + i);
			glBindTexture(GL_TEXTURE_2D, shadow_layers[i]);
		}

		if(check_ogl_error()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind shadow layer buffers!" << std::endl;
			errorlogger("ERROR: Failed to bind shadow layer buffers!");
			return false;
		}
	}

	return true;
}

bool Renderer::upload_view_position(const glm::vec3& position)const{
	glm::vec3 view_position = glm::vec3(view * glm::vec4(position, 1.0));
	glBindBuffer(GL_UNIFORM_BUFFER, get_uniform_buffer("camera_data"));
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(view_position)); 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload view position!" << std::endl;
		errorlogger("ERROR: Failed to upload view position!");
		return false;
	}

	return true;
}

bool Renderer::render_lights(const glm::vec3& position){
	glBindFramebuffer(GL_FRAMEBUFFER, light_fbo);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	clear();

	if (!upload_light_data()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload light data!" << std::endl;
		errorlogger("ERROR: Failed to upload light data!");
		return false;
	}

	if (!upload_view_position(position)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind view position for lights!" << std::endl;
		errorlogger("ERROR: Failed to bind view position for lights!");
		return false;
	}

	if (!bind_g_data(LIGHT_DIRECTIONAL)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind g_data for directional lights!" << std::endl;
		errorlogger("ERROR: Failed to bind g_data for directional lights!");
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
	
	if (!render_spot_lights()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render spot lights!" << std::endl;
		errorlogger("ERROR: Failed to render spot lights!");
		return false;
	}

	glDisable(GL_BLEND);

	return true;
}

bool Renderer::upload_light_data(){
	if (!upload_dir_lights_data()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload directional light data!" << std::endl;
		errorlogger("ERROR: Failed to upload directional light data!");
		return false;
	}

	if (!upload_point_lights_data()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload point light data!" << std::endl;
		errorlogger("ERROR: Failed to upload point light data!");
		return false;
	}

	if (!upload_spot_lights_data()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload spot light data!" << std::endl;
		errorlogger("ERROR: Failed to upload spot light data!");
		return false;
	}

	return true;
}

bool Renderer::upload_dir_lights_data(){
	num_active_dir_lights = 0;
	GLuint uniform_buffer = get_uniform_buffer("dir_lights");
	for (auto light_context = dir_lights.begin(); light_context != dir_lights.end(); ++light_context) {
		auto context = light_context->lock();
		if (!context) {
			SDL_Log("Directional light context expired, removing from renderer...");
			point_lights.erase(light_context);
			continue;
		}

		if(!context->setup_base_uniforms(uniform_buffer, view, num_active_dir_lights)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to setup directional light uniforms!" << std::endl;
			errorlogger("ERROR: Failed to setup directional light uniforms!");
			return false;
		}

		++num_active_dir_lights;

		if (num_active_dir_lights == Utility_consts::MAX_DIR_LIGHTS) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Number of directional lights is greater than max, some lights might get culled!" << std::endl;
			errorlogger("WARNING: Number of directional lights is greater than max, some lights might get culled!");
			break;
		}
		
	}
   
	return true;
}

bool Renderer::upload_point_lights_data(){
	num_active_point_lights = 0;
	GLuint uniform_buffer = get_uniform_buffer("point_lights");
	for (auto light_context = point_lights.begin(); light_context != point_lights.end(); ++light_context) {
		auto context = light_context->lock();
		if (!context) {
			SDL_Log("Point light context expired, removing from renderer...");
			point_lights.erase(light_context);
			continue;
		}

		if(!context->setup_base_uniforms(uniform_buffer, view, num_active_point_lights)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render point light!" << std::endl;
			errorlogger("ERROR: Failed to render point light!");
			return false;
		}

		++num_active_point_lights;

		if (num_active_point_lights == Utility_consts::MAX_POINT_LIGHTS) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Number of point lights is greater than max, some lights might get culled!" << std::endl;
			errorlogger("WARNING: Number of point lights is greater than max, some lights might get culled!");
			break;
		}
		
	}
   
	return true;
}

bool Renderer::upload_spot_lights_data(){
	num_active_spot_lights = 0;
	GLuint uniform_buffer = get_uniform_buffer("spot_lights");
	for (auto light_context = spot_lights.begin(); light_context != spot_lights.end(); ++light_context) {
		auto context = light_context->lock();
		if (!context) {
			SDL_Log("Spot light context expired, removing from renderer...");
			point_lights.erase(light_context);
			continue;
		}

		if(!context->setup_base_uniforms(uniform_buffer, view, num_active_spot_lights)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to setup spot light uniforms!" << std::endl;
			errorlogger("ERROR: Failed to setup spot light uniforms!");
			return false;
		}

		++num_active_spot_lights;

		if (num_active_spot_lights == Utility_consts::MAX_SPOT_LIGHTS) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Number of directional lights is greater than max, some lights might get culled!" << std::endl;
			errorlogger("WARNING: Number of directional lights is greater than max, some lights might get culled!");
			break;
		}
		
	}
   
	return true;
}

bool Renderer::render_dir_lights(){
	if (!render_quad(num_active_dir_lights)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render quad batch for directional lights!" << std::endl;
		errorlogger("ERROR: Failed to render quad batch for directional lights!");
		return false;
	}

	return true;
}

bool Renderer::render_point_lights(){
	if (!render_cube(num_active_point_lights)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render cube batch for point lights!" << std::endl;
		errorlogger("ERROR: Failed to render cube batch for point lights!");
		return false;
	}

	return true;
}

bool Renderer::render_spot_lights(){
	if (!render_cube(num_active_spot_lights)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render cube batch for spot lights!" << std::endl;
		errorlogger("ERROR: Failed to render cube batch for spot lights!");
		return false;
	}

	return true;
}

bool Renderer::set_viewport_window()const{
	glViewport(0, 0, window_size.x, window_size.y);

	return true;
}

bool Renderer::set_viewport_resolution()const{
	glViewport(0, 0, resolution.x, resolution.y);

	return true;
}


bool Renderer::upload_res_data()const{
	glBindBuffer(GL_UNIFORM_BUFFER, get_uniform_buffer("resolution_data"));
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec2), glm::value_ptr(window_size));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), sizeof(glm::vec2), glm::value_ptr(resolution));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);  

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload light data vector!" << std::endl;
		errorlogger("ERROR: Failed to upload light data vector!");
		return false;
	}
	return true;
}

/* ================================================================== GeomGeom */

bool Renderer::upload_plane_data()const{
	glm::vec2 plane_data = {near_plane, far_plane};
	glBindBuffer(GL_UNIFORM_BUFFER, get_uniform_buffer("plane_data"));
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec2), glm::value_ptr(plane_data));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);  

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload plane data!" << std::endl;
		errorlogger("ERROR: Failed to upload plane data!");
		return false;
	}
	return true;
}

bool Renderer::render_geometry(){
	glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_FRONT);
	glDepthMask(GL_TRUE);
	clear();

	if(check_ogl_error()){
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

#if ENABLE_BULLET_DEBUG
	primitive_line_shader->use();
	if (!render_primitive_line_geometry()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render primitive line geometry!" << std::endl;
		errorlogger("ERROR: Failed to render primitive line geometry!");
		return false;
	}
#endif
	
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to detach geometry rendering!" << std::endl;
		errorlogger("ERROR: Failed to detach geometry rendering!");
		return false;
	}

	return true;
}

bool Renderer::render_static_geomety()const{
	for (auto& static_context : static_geom) {
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
	for (auto& static_colored_context : static_colored_geom) {
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
	for (auto& anim_context : animated_geom) {
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
	for (auto& anim_colored_context : animated_colored_geom) {
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

#if ENABLE_BULLET_DEBUG
bool Renderer::render_primitive_line_geometry(){
	if (!line->setup_base_uniforms(primitive_line_shader)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to setup base uniform for line primitive!" << std::endl;
		errorlogger("ERROR: Failed to setup base uniform for line primitive!");
		return false;
	}

	GLuint instance = 0;
	auto context_iterator = lines.begin();
	while(context_iterator != lines.end()){
		glUniform3fv(primitive_line_shader->load_uniform_location("colors", instance), 1, (float*)&(context_iterator->color));
		glUniformMatrix4fv(primitive_line_shader->load_uniform_location("models", instance),
					 1, 
					 GL_FALSE, 
					 glm::value_ptr(context_iterator->model_matrix));
		lines.erase(context_iterator++);
		++instance;

		if (instance >= Renderer_consts::BATCH_SIZE){
			if (!ogl_render_geometry(line->get_main_context(), instance)) {
				std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render line batch geometry!" << std::endl;
				errorlogger("ERROR: Failed to render line batch geometry!");
				return false;
			}

			instance = 0;
		}
	}

	if (instance > 0){
		if (!ogl_render_geometry(line->get_main_context(), instance)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render line batch geometry!" << std::endl;
			errorlogger("ERROR: Failed to render line batch geometry!");
			return false;
		}
	}
   
	return true;
}
#endif


bool Renderer::render_base_geometry(const Rendering_context_ptr& context, 
								const Shader_ptr& shader)const{
	context->setup_base_uniforms(shader);

	GLuint instance = 0;
	for (auto context_iterator = context->instance_uniform_setups.begin(); context_iterator != context->instance_uniform_setups.end(); ++context_iterator) {
		if (!context_iterator->second(shader, instance, false)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to setup instance uniforms!" << std::endl;
			errorlogger("ERROR: Failed to setup instance uniforms!");
			return false;
		}

		++instance;

		if (instance >= Renderer_consts::BATCH_SIZE){
			if (!ogl_render_geometry(context, instance)){
				std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render ogl shadow geometry!" << std::endl;
				errorlogger("ERROR: Failed to render ogl shadow geometry!");
				return false;
			}

			instance = 0;
		}
	} 

	if (instance > 0){
		if (!ogl_render_geometry(context, instance)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render ogl shadow geometry!" << std::endl;
			errorlogger("ERROR: Failed to render ogl shadow geometry!");
			return false;
		}
	}
   
	return true;
}

bool Renderer::ogl_render_geometry(const Rendering_context_ptr& context, GLuint instances)const{
	glPolygonMode(GL_FRONT_AND_BACK, context->render_mode);
	glBindVertexArray(context->VAO);
	if (context->render_elements) {
		glDrawElementsInstanced(context->primitive_type, 
								context->num_vertices, 
								GL_UNSIGNED_INT, 
								0, 
								instances);
	}
	else{
		glDrawArraysInstanced(context->primitive_type, 0, context->num_vertices, instances);
	}

	glBindVertexArray(0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: OpenGL error, failed to draw geometry!" << std::endl;
		errorlogger("ERROR: OpenGL error, failed to draw geometry!");
		return false;
	}

	return true;
}

/* ================================================================== BloomBloom */

bool Renderer::apply_bloom(GLuint bloom_fbo, GLuint color_texture){
	glBindFramebuffer(GL_FRAMEBUFFER, bloom_fbo);
	clear();

	bloom_shader->use();
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(FXAA_shader->load_uniform_location("color"), 0);
	glBindTexture(GL_TEXTURE_2D, color_texture); 
	
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind bloom buffers!" << std::endl;
		errorlogger("ERROR: Failed to bind bloom buffers!");
		return false;
	}

	if (!render_quad()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render bloom quad!" << std::endl;
		errorlogger("ERROR: Failed to render bloom quad!");
		return false;
	}

	return true;
}

bool Renderer::toggle_post_bloom() {
	if (use_post_bloom) {
		use_post_bloom = false;
	}
	else{
		use_post_bloom = true;
	}

	return true;
}

bool Renderer::toggle_pre_bloom() {
	if (use_pre_bloom) {
		use_pre_bloom = false;
	}
	else{
		use_pre_bloom = true;
	}

	return true;
}

/* ================================================================== AAAA */

bool Renderer::apply_AA()const{
	glBindFramebuffer(GL_FRAMEBUFFER, AA_fbo);
	clear();

	FXAA_shader->use();
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(FXAA_shader->load_uniform_location("colortexture"), 0);
	glBindTexture(GL_TEXTURE_2D, light_color_buffer); 
	
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind AA buffers!" << std::endl;
		errorlogger("ERROR: Failed to bind AA buffers!");
		return false;
	}

	if (!render_quad()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render AA quad!" << std::endl;
		errorlogger("ERROR: Failed to render AA quad!");
		return false;
	}

	return true;
}

bool Renderer::toggle_aliasing() {
	if(use_AA) {
		use_AA = false;
	}
	else{
		use_AA = true;
	}

	return true;
}

/* ======================================================= SSAOSSAO */

GLfloat Renderer::lerp(GLfloat a, GLfloat b, GLfloat f)const{
    return a + f * (b - a);
} 

bool Renderer::apply_SSAO(){
	glBindFramebuffer(GL_FRAMEBUFFER, SSAO_fbo);
	clear();

	SSAO_shader->use();
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(SSAO_shader->load_uniform_location("g_position"), 0);
	glBindTexture(GL_TEXTURE_2D, g_position);

	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to setup SSAO rendering!" << std::endl;
		errorlogger("ERROR: Failed to setup SSAO rendering!");
		return false;
	}

	if (!render_quad()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render SSAO quad!" << std::endl;
		errorlogger("ERROR: Failed to render SSAO quad!");
		return false;
	}

	if (!blur_texture(2, SSAO_buffer, SSAO_fbo)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to blur SSAO_buffer!" << std::endl;
		errorlogger("ERROR: Failed to blur SSAO_buffer!");
		return false;
	}

	return true;
}

bool Renderer::toggle_ambient_occlusion() {
	if(use_SSAO) {
		use_SSAO = false;
	}
	else{
		use_SSAO = true;
	}

	if (!upload_settings()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload new settings!" << std::endl;
		errorlogger("ERROR: Failed to upload new settings!");
		return false;
	}

	return true;
}

/* ================================================================== ShadowShadow */

bool Renderer::render_shadow_geometry(const Rendering_context_ptr& context){
	GLuint instance = 0;
	for (auto context_iterator = context->instance_uniform_setups.begin(); context_iterator != context->instance_uniform_setups.end(); ++context_iterator) {
		if (!context_iterator->second(LFST_cull_shader, instance, true)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to setup instance uniforms!" << std::endl;
			errorlogger("ERROR: Failed to setup instance uniforms!");
			return false;
		}

		++instance;

		if (instance >= Renderer_consts::BATCH_SIZE){
			if (!ogl_render_shadow_geometry(context, instance)){
				std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render ogl shadow geometry!" << std::endl;
				errorlogger("ERROR: Failed to render ogl shadow geometry!");
				return false;
			}

			instance = 0;
		}
	} 

	if (instance > 0){
		if (!ogl_render_shadow_geometry(context, instance)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render ogl shadow geometry!" << std::endl;
			errorlogger("ERROR: Failed to render ogl shadow geometry!");
			return false;
		}
	}
   
	return true;
}

bool Renderer::copy_depth(GLuint source_fbo, GLuint target_fbo){
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target_fbo);
	glBlitFramebuffer(0, 0, resolution.x, resolution.y, 0, 0, resolution.x, resolution.y, 
                  GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to copy depth buffer!" << std::endl;
		errorlogger("ERROR: Failed to copy depth buffer!");
		return false;
	}

	return true;
}

bool Renderer::ogl_render_shadow_geometry(const Rendering_context_ptr& context, GLuint instances){
	glPolygonMode(GL_FRONT_AND_BACK, context->render_mode);
	glBindVertexArray(context->VAO);

	for (GLuint i = 0; i < Renderer_consts::SHADOW_LAYERS; ++i) {
		if (i == 0) {
			glUniform1i(LFST_cull_shader->load_uniform_location("use_mask"), 0);
		}
		else if (i == 1) {
			glUniform1i(LFST_cull_shader->load_uniform_location("use_mask"), 1);

			glActiveTexture(GL_TEXTURE0);
			glUniform1i(LFST_cull_shader->load_uniform_location("discard_mask"), 0);
			glBindTexture(GL_TEXTURE_2D, shadow_front_cull_buffers[i-1]);
		}
		else{
			glActiveTexture(GL_TEXTURE0);
			glUniform1i(LFST_cull_shader->load_uniform_location("discard_mask"), 0);
			glBindTexture(GL_TEXTURE_2D, shadow_front_cull_buffers[i-1]);
		}

		if(check_ogl_error()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind discard mask!" << std::endl;
			errorlogger("ERROR: Failed to bind discard mask!");
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, shadow_front_cull_fbos[i]);
		glCullFace(GL_FRONT);
		if (clear_shadow_layers[i]) {
			clear();
		}

		if (context->render_elements) {
			glDrawElementsInstanced(context->primitive_type, 
								context->num_vertices, 
								GL_UNSIGNED_INT, 
								0, 
								instances);
		}
		else{
			glDrawArraysInstanced(context->primitive_type, 0, context->num_vertices, instances);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, shadow_back_cull_fbos[i]);
		glCullFace(GL_BACK);
		if (clear_shadow_layers[i]) {
			clear();
			clear_shadow_layers[i] = false;
		}

		if (context->render_elements) {
			glDrawElementsInstanced(context->primitive_type, 
								context->num_vertices, 
								GL_UNSIGNED_INT, 
								0, 
								instances);
		}
		else{
			glDrawArraysInstanced(context->primitive_type, 0, context->num_vertices, instances);
		}
	}

	glBindVertexArray(0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: OpenGL error, failed to draw shadow geometry!" << std::endl;
		errorlogger("ERROR: OpenGL error, failed to draw shadow geometry!");
		return false;
	}

	return true;
}

bool Renderer::render_shadow_cull_layers(){
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glFrontFace(GL_CCW);
	for (GLuint i = 0; i < Renderer_consts::SHADOW_LAYERS; ++i) {
		clear_shadow_layers[i] = true;
	} 
	LFST_cull_shader->use();
	for (auto& object_context : animated_geom){
		auto context = object_context.lock();
		if (!context) {
			SDL_Log("Animated geometry context expired, removing from renderer...");
			/* TODO::Remove context */
			continue;
		}
		else if (!render_shadow_geometry(context)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render cull layers for animated geometry!" << std::endl;
			errorlogger("ERROR: Failed to render cull layers for animated geometry!");
			return false;
		}
	}

	for (auto& object_context : animated_colored_geom){
		auto context = object_context.lock();
		if (!context) {
			SDL_Log("Animated colored geometry context expired, removing from renderer...");
			/* TODO::Remove context */
			continue;
		}
		else if (!render_shadow_geometry(context)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render cull layers for animated colored geometry!" << std::endl;
			errorlogger("ERROR: Failed to render cull layers for animated colored geometry!");
			return false;
		}
	}

	for (auto& object_context : static_geom){
		auto context = object_context.lock();
		if (!context) {
			SDL_Log("Static geometry context expired, removing from renderer...");
			// TODO::Remove context 
			continue;
		}
		else if (!render_shadow_geometry(context)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render cull layers for static geometry!" << std::endl;
			errorlogger("ERROR: Failed to render cull layers for static geometry!");
			return false;
		}
	}

	for (auto& object_context : static_colored_geom){
		auto context = object_context.lock();
		if (!context) {
			SDL_Log("Static colored geometry context expired, removing from renderer...");
			// TODO::Remove context 
			continue;
		}
		else if (!render_shadow_geometry(context)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render cull layers for animated geometry!" << std::endl;
			errorlogger("ERROR: Failed to render cull layes for static colored geometry!");
			return false;
		}
	}

	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	return true;
}

bool Renderer::generate_shadow_layers(){
	LFST_layer_shader->use();
	for (GLuint i = 0; i < Renderer_consts::SHADOW_LAYERS; ++i) {
		glBindFramebuffer(GL_FRAMEBUFFER, shadow_layer_fbos[i]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadow_front_cull_buffers[i]);
		glUniform1i(LFST_layer_shader->load_uniform_location("front_culled_map"), 0);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, shadow_back_cull_buffers[i]);
		glUniform1i(LFST_layer_shader->load_uniform_location("back_culled_map"), 1);

		if(check_ogl_error()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind culled depth buffers for shadow layer pass!" << std::endl;
			errorlogger("ERROR: Failed to bind culled depth buffers for shadow layer pass!");
			return false;
		}

		if (!render_quad()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render shadow layer quad!" << std::endl;
			errorlogger("ERROR: Failed to render shadow layer quad!");
			return false;
		}
	}

	return true;
}

bool Renderer::apply_shadows(){
	if (!render_shadow_cull_layers()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render shadow cull layers!" << std::endl;
		errorlogger("ERROR: Failed to render shadow cull layers!");
		return false;
	}

	if (!generate_shadow_layers()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to generate shadow layers!" << std::endl;
		errorlogger("ERROR: Failed to generate shadow layers!");
		return false;
	}

	return true;
}

bool Renderer::toggle_shadows(){
	if (use_shadows){
		use_shadows = false;
	}
	else{
		use_shadows = true;
	}

	if (!upload_settings()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload new settings!" << std::endl;
		errorlogger("ERROR: Failed to upload new settings!");
		return false;
	}

	return true;
}

/* ======================================================= GenericGeneric */

bool Renderer::render_all(const Camera_ptr& camera){
	if (!update_window_size()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to update window size!"<< std::endl;
		errorlogger("ERROR: Failed to update window size!");
		return false;
	}

	if (!set_viewport_resolution()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to set viewport resolution!"<< std::endl;
		errorlogger("ERROR: Failed to set viewport resolution!");
		return false;
	}

	update_view_matrix(camera->get_position(), camera->get_target(), camera->get_up_dir());

	if (!upload_view_matrix()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload view matrix!"<< std::endl;
		errorlogger("ERROR: Failed to upload view matrix!");
		return false;
	}

	/*
	use_default_buffer();
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	clear();

	final_shader->use();
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(final_shader->load_uniform_location("colors"), 0);
	glBindTexture(GL_TEXTURE_2D, shadow_layers[2]);


	if (!render_quad()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render bloom quad!" << std::endl;
		errorlogger("ERROR: Failed to render bloom quad!");
		return false;
	}

	glDisable(GL_BLEND);*/

	/*glm::vec4 vectortest = {0.0, 100.0, 200.0, 1.0};

	vectortest = projection * view * vectortest;

	vectortest /= vectortest.w;
	//vectortest *= 0.5;
	//vectortest += 0.5;
	std::cout << vectortest.x << " : " << vectortest.y << " : " << vectortest.z << " : " << vectortest.w << " RESULT!" << std::endl; 
	*/

	if(!render_geometry()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render geometry!"<< std::endl;
		errorlogger("ERROR: Failed to render geometry!");
		return false;
	}

	if (use_shadows){
		if (!apply_shadows()){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to apply shadows!"<< std::endl;
			errorlogger("ERROR: Failed to apply shadows!");
			return false;
		}
	}

	if (use_SSAO){
		if (!apply_SSAO()){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to apply ambient occlusion!"<< std::endl;
			errorlogger("ERROR: Failed to apply ambient occlusion!");
			return false;
		}
	}

	if(!render_lights(camera->get_position())){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render lights!" << std::endl;
		errorlogger("ERROR: Failed to render lights!");
		return false;
	}

	if (!apply_ppe()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to apply post processing effects!"<< std::endl;
		errorlogger("ERROR: Failed to apply post processing effects!");
		return false;
	}

	if (!ppe_blend()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render post processing effects!"<< std::endl;
		errorlogger("ERROR: Failed to render post processing effects!");
		return false;
	}

	if (!hdr_to_default_buffer()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render HDR to default buffer!"<< std::endl;
		errorlogger("ERROR: Failed to render HDR to default buffer!");
		return false;
	}

	return true;
}

bool Renderer::apply_ppe(){
	if (use_AA){
		if (!apply_AA()){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to apply anti aliasing!"<< std::endl;
			errorlogger("ERROR: Failed to apply anti aliasing!");
			return false;
		}
	}

	if (use_pre_bloom) {
		if (!apply_bloom(pre_bloom_fbo, g_albedo_spec)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to apply pre bloom!" << std::endl;
			errorlogger("ERROR: Failed to apply pre bloom!");
			return false;
		}

		if (!blur_texture(2, pre_bloom_buffer, pre_bloom_fbo)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to blur pre_bloom_buffer!" << std::endl;
			errorlogger("ERROR: Failed to blur pre_bloom_buffer!");
			return false;
		}

	}

	if (use_post_bloom) {
		if (!apply_bloom(post_bloom_fbo, light_color_buffer)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to apply post bloom!" << std::endl;
			errorlogger("ERROR: Failed to apply bloom!");
			return false;
		}

		if (!blur_texture(2, post_bloom_buffer, post_bloom_fbo)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to blur post_bloom_buffer!" << std::endl;
			errorlogger("ERROR: Failed to blur post_bloom_buffer!");
			return false;
		}
	}

	return true;
}

bool Renderer::ppe_blend(){
	glBindFramebuffer(GL_FRAMEBUFFER, ppe_blend_fbo);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	clear();

	ppe_blend_shader->use();
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(ppe_blend_shader->load_uniform_location("colors"), 0);
	
	if (use_pre_bloom) {
		glBindTexture(GL_TEXTURE_2D, pre_bloom_buffer);
		if(check_ogl_error()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind blurred buffer!" << std::endl;
			errorlogger("ERROR: Failed to bind blurred buffer!");
			return false;
		}

		if (!render_quad()){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render pre bloom quad!" << std::endl;
			errorlogger("ERROR: Failed to render pre bloom quad!");
			return false;
		}
	}

	if (use_post_bloom) {
		glBindTexture(GL_TEXTURE_2D, post_bloom_buffer);
		if(check_ogl_error()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind blurred buffer!" << std::endl;
			errorlogger("ERROR: Failed to bind blurred buffer!");
			return false;
		}

		if (!render_quad()){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render post bloom quad!" << std::endl;
			errorlogger("ERROR: Failed to render post bloom quad!");
			return false;
		}
	}

	if (use_AA) {
		glBindTexture(GL_TEXTURE_2D, AA_buffer);
	}
	else{
		glBindTexture(GL_TEXTURE_2D, light_color_buffer);
	}
	

	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind AA buffer!" << std::endl;
		errorlogger("ERROR: Failed to bind AA buffer!");
		return false;
	}

	if (!render_quad()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render AA quad!" << std::endl;
		errorlogger("ERROR: Failed to render AA quad!");
		return false;
	}

	glDisable(GL_BLEND);

	return true;
}

bool Renderer::hdr_to_default_buffer(){
	if (!use_default_buffer()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to set framebuffer target to default buffer!" << std::endl;
		errorlogger("ERROR: Failed to set framebuffer target to default buffer!");
		return false;
	}

	HDR_shader->use();
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(HDR_shader->load_uniform_location("colors"), 0);
	glBindTexture(GL_TEXTURE_2D, ppe_blend_buffer);

	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind PPE buffer!" << std::endl;
		errorlogger("ERROR: Failed to bind PPE buffer!");
		return false;
	}

	if (!render_quad()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render HDR quad!" << std::endl;
		errorlogger("ERROR: Failed to render HDR quad!");
		return false;
	}

	return true;
}



bool Renderer::render_quad()const{
	glBindVertexArray(quad_VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind quad!" << std::endl;
		errorlogger("ERROR: Failed to bind quad!");
		return false;
	}
	return true;
}

bool Renderer::save_settings(){
	std::cout << "Saving display settings...\n" << std::endl;
	std::ofstream contentf (Utility_vars::folder_path + DISPLAY_SETTINGS_FILE_PATH, std::ios::binary);

	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for display settings!" << std::endl;
		errorlogger("ERROR: Failed to open content file for display settings!");
		return false;
	}

	contentf.write(reinterpret_cast<const char *>(&ortographic), sizeof(GLboolean));
	contentf.write(reinterpret_cast<const char *>(&use_vsync), sizeof(GLboolean));
	contentf.write(reinterpret_cast<const char *>(&use_fullscreen), sizeof(GLboolean));
	contentf.write(reinterpret_cast<const char *>(&use_mouse), sizeof(GLboolean));
	contentf.write(reinterpret_cast<const char *>(&use_pre_bloom), sizeof(GLboolean));
	contentf.write(reinterpret_cast<const char *>(&use_post_bloom), sizeof(GLboolean));
	contentf.write(reinterpret_cast<const char *>(&use_AA), sizeof(GLboolean));
	contentf.write(reinterpret_cast<const char *>(&use_SSAO), sizeof(GLboolean));
	contentf.write(reinterpret_cast<const char *>(&use_shadows), sizeof(GLboolean));

	contentf.write(reinterpret_cast<const char *>(&near_plane), sizeof(GLfloat));
	contentf.write(reinterpret_cast<const char *>(&far_plane), sizeof(GLfloat));

	contentf.write(reinterpret_cast<const char *>(&lower_bloom_threshold), sizeof(GLfloat));
	contentf.write(reinterpret_cast<const char *>(&higher_bloom_threshold), sizeof(GLfloat));

	contentf.write(reinterpret_cast<const char *>(&gamma), sizeof(GLfloat));
	contentf.write(reinterpret_cast<const char *>(&exposure), sizeof(GLfloat));

	contentf.write(reinterpret_cast<const char *>(&window_size.x), sizeof(GLfloat));
	contentf.write(reinterpret_cast<const char *>(&window_size.y), sizeof(GLfloat));

	contentf.write(reinterpret_cast<const char *>(&resolution.x), sizeof(GLfloat));
	contentf.write(reinterpret_cast<const char *>(&resolution.y), sizeof(GLfloat));

	contentf.close();
	std::cout << "Display settings saved!\n" << std::endl;
	return true;
}

bool Renderer::load_settings(){
	std::ifstream contentf (Utility_vars::folder_path + DISPLAY_SETTINGS_FILE_PATH, std::ios::binary);

	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for display settings!" << std::endl;
		errorlogger("ERROR: Failed to open content file for display settings!");
		return false;
	}

	contentf.read(reinterpret_cast<char *>(&ortographic), sizeof(GLboolean));
	contentf.read(reinterpret_cast<char *>(&use_vsync), sizeof(GLboolean));
	contentf.read(reinterpret_cast<char *>(&use_fullscreen), sizeof(GLboolean));
	contentf.read(reinterpret_cast<char *>(&use_mouse), sizeof(GLboolean));
	contentf.read(reinterpret_cast<char *>(&use_pre_bloom), sizeof(GLboolean));
	contentf.read(reinterpret_cast<char *>(&use_post_bloom), sizeof(GLboolean));
	contentf.read(reinterpret_cast<char *>(&use_AA), sizeof(GLboolean));
	contentf.read(reinterpret_cast<char *>(&use_SSAO), sizeof(GLboolean));
	contentf.read(reinterpret_cast<char *>(&use_shadows), sizeof(GLboolean));

	contentf.read(reinterpret_cast<char *>(&near_plane), sizeof(GLfloat));
	contentf.read(reinterpret_cast<char *>(&far_plane), sizeof(GLfloat));

	contentf.read(reinterpret_cast<char *>(&lower_bloom_threshold), sizeof(GLfloat));
	contentf.read(reinterpret_cast<char *>(&higher_bloom_threshold), sizeof(GLfloat));

	contentf.read(reinterpret_cast<char *>(&gamma), sizeof(GLfloat));
	contentf.read(reinterpret_cast<char *>(&exposure), sizeof(GLfloat));

	contentf.read(reinterpret_cast<char *>(&window_size.x), sizeof(GLfloat));
	contentf.read(reinterpret_cast<char *>(&window_size.y), sizeof(GLfloat));

	contentf.read(reinterpret_cast<char *>(&resolution.x), sizeof(GLfloat));
	contentf.read(reinterpret_cast<char *>(&resolution.y), sizeof(GLfloat));

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

#if ENABLE_BULLET_DEBUG
bool Renderer::add_context(Line_data context){
	lines.push_back(context);

	return true;
}
#endif

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

bool Renderer::add_context(const Rendering_context_light_ptr& context) {

	Rendering_context_light_weak context_weak = context;

	if (context){
		switch (context->shader_type) {
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

bool Renderer::use_default_buffer()const{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bind default_framebuffer!" << std::endl;
		errorlogger("ERROR: Failed to bind default_framebuffer!");
		return false;
	}

	if (!set_viewport_window()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to set viewport to window size!" << std::endl;
		errorlogger("ERROR: Failed to set viewport to window size!");
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

bool Renderer::update_projection_matrix(){
	if (ortographic) {
		projection = glm::ortho(0.0f, window_size.x, 0.0f, window_size.y, near_plane, far_plane);
	}
	else{
		projection = glm::perspective(45.0f, window_size.x / window_size.y, near_plane, far_plane);
	}

	if (!upload_plane_data()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload plane data!" << std::endl;
		errorlogger("ERROR: Failed to upload plane data!");
		return false;
	}

	return true;
}

bool Renderer::upload_projection_matrix()const{
	glBindBuffer(GL_UNIFORM_BUFFER, get_uniform_buffer("matrices"));
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);  

	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload projection matrix!" << std::endl;
		errorlogger("ERROR: Failed to upload projection matrix!");
		return false;
	}

	return true;
}

bool Renderer::toggle_mouse(){
	GLint error = 0;
	if (use_mouse) {
		use_mouse = false;
		error = SDL_ShowCursor(0);
	}
	else{
		use_mouse = true;
		error = SDL_ShowCursor(1);
	}

	if (error < 0){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to toggle mouse visibility!" << std::endl;
		SDLerrorLogger("SDL_ShowCursor");
		return false;
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
	return true;
}

bool Renderer::set_window_size(GLuint width, GLuint height){
	window_size.x = (GLfloat) width;
	window_size.y = (GLfloat) height;
	SDL_SetWindowSize(window, width, height);
	if (!update_projection_matrix()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to update projection matrix!" << std::endl;
		errorlogger("ERROR: Failed to update projection matrix!");
		return false;
	}

	if (!upload_projection_matrix()) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to upload projection matrix!" << std::endl;
		errorlogger("ERROR: Failed to upload projection matrix!");
		return false;
	}
	return true;
}

bool Renderer::update_window_size(){
	GLint width;
	GLint height;
	SDL_GetWindowSize(window, &width, &height);
	if (width != window_size.x || height != window_size.y) {
		if (!set_window_size(width, height)){
			std::cout << "ERROR: Failed to set swindow size!" << std::endl;
			errorlogger("ERROR: Failed to set window size!");
			return false;
		}

		if (!upload_res_data()) {
			std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to upload resolution data in renderer!" << std::endl;
			errorlogger("ERROR: Failed to upload resolution data in renderer!");
			return false;
		}
	}
	return true;
}

bool Renderer::update_resolution(const glm::vec2& new_res){
	if (resolution.x != new_res.x || resolution.y != new_res.y) {
		resolution.x = new_res.x;
		resolution.y = new_res.y;

		delete_buffers();
		init_framebuffers();
		if(check_ogl_error()) {
			std::cout << "ERROR: Failed to reset framebuffers!" << std::endl;
			errorlogger("ERROR: Failed to reset framebuffers!");
			return false;
		}
	}
	return true;
}

bool Renderer::enable_vsync(){
	if (!(SDL_GL_SetSwapInterval(1) < 0)){
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: SDL_GL_SetSwapInterval could not enable vsync! SDL Error: " << SDL_GetError() << std::endl;
		SDLerrorLogger("SDL_GL_SetSwapInterval");
		return false;
	}
}

bool Renderer::disable_vsync(){
	if (!(SDL_GL_SetSwapInterval(0) < 0)){
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

bool Renderer::delete_buffers() {
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

	glDeleteFramebuffers(1, &AA_fbo);
	glDeleteTextures(1, &AA_buffer);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to delete AA_buffer objects!" << std::endl;
		errorlogger("ERROR: Failed to delete AA_buffer objects!");
		return false;
	}

	glDeleteFramebuffers(1, &SSAO_fbo);
	glDeleteTextures(1, &SSAO_buffer);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to delete SSAO_buffer objects!" << std::endl;
		errorlogger("ERROR: Failed to delete SSAO_buffer objects!");
		return false;
	}

	glDeleteFramebuffers(2, &bb_fbos[0]);
	glDeleteTextures(2, &bb_buffers[0]);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to delete bloom_buffer objects!" << std::endl;
		errorlogger("ERROR: Failed to delete bloom_buffer objects!");
		return false;
	}

	glDeleteFramebuffers(1, &light_fbo);
	glDeleteTextures(1, &light_color_buffer);
	glDeleteRenderbuffers(1, &light_rbo_depth);
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to delete light_buffer objects!" << std::endl;
		errorlogger("ERROR: Failed to delete light_buffer objects!");
		return false;
	}

	return true;
}

bool Renderer::delete_uniform_buffers(){
	for (auto& buffer : uniform_buffers) {
		glDeleteBuffers(1, &buffer.second);
	}

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to delete uniform buffer objects!" << std::endl;
		errorlogger("ERROR: Failed to delete uniform buffer objects!");
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
	glBindBuffer(GL_UNIFORM_BUFFER, get_uniform_buffer("matrices"));
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

bool Renderer::blur_texture(GLuint amount, GLuint texture, GLuint texture_fbo){
	if ((amount % 2) != 0) {
		++amount;
	}

	GLboolean first_shader_set = false;
	GLboolean second_shader_set = false;
	GLuint target_texture = texture;

	for (GLuint i = 0; i < amount; i++) {
		if (!first_shader_set && (i < amount/2)) {
			first_shader_set = true;
			vertical_blur_shader->use();
		}
		else if (!second_shader_set && (i >= amount/2)) {
			second_shader_set = true;
			horizontal_blur_shader->use();
		}

		if (i == 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindFramebuffer(GL_FRAMEBUFFER, bb_fbos[1 - (i % 2)]); 
		}else if (i > 0 && i < amount - 1) {
			target_texture = bb_buffers[i % 2];
			glBindFramebuffer(GL_FRAMEBUFFER, bb_fbos[1 - (i % 2)]); 
		}
		else if (i == amount - 1) {
			target_texture = bb_buffers[i % 2];
			glBindFramebuffer(GL_FRAMEBUFFER, texture_fbo); 
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, target_texture); 
		if (!render_quad()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to render light quad for blur calculations!" << std::endl;
			errorlogger("ERROR: Failed to render light quad for blur calculations!");
			return false;
		}
	}

	return true;
}

bool Renderer::render_cube(GLuint instances)const{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(cube_VAO);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW); 

	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, instances);

	glDisable(GL_CULL_FACE);
	glBindVertexArray(0);
	
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to configure cube rendering!" << std::endl;
		errorlogger("ERROR: Failed to configure cube rendering!");
		return false;
	}

	return true;
}

bool Renderer::render_quad(GLuint instances)const{
	glBindVertexArray(quad_VAO);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, instances);
	glBindVertexArray(0);
	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind quad!" << std::endl;
		errorlogger("ERROR: Failed to bind quad!");
		return false;
	}
	return true;
}

bool Renderer::upload_settings()const{
	glm::vec4 shadow_settings = {use_SSAO, use_shadows, 0.0, 0.0};
	glm::vec4 ppe_settings = {lower_bloom_threshold, higher_bloom_threshold, gamma, exposure};
	glBindBuffer(GL_UNIFORM_BUFFER, get_uniform_buffer("settings"));
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(shadow_settings)); 
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(ppe_settings)); 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload settings!" << std::endl;
		errorlogger("ERROR: Failed to upload settings!");
		return false;
	}

	return true;
}

bool Renderer::make_viewport_matrix(glm::mat3& matrix){
	if(!window_initialized){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Window not initialized, cannot construct viewport matrix!"<<std::endl;
		errorlogger("ERROR: Window not initialized, cannot construct viewport matrix!");
		return false;
	}

	glm::mat3 viewport = glm::mat3();

	viewport[0][0] = window_size.x/2;
	viewport[1][1] = window_size.y/2;
	viewport[2][2] = 1.0f;

	viewport[2][0] = window_size.x/2 + window_init_pos.x;
	viewport[2][1] = window_size.y/2 + window_init_pos.y;

	matrix = viewport;

	return true;
}

bool Renderer::init_tiles(){
	GLuint num_tiles_width = resolution.x / Renderer_consts::TILESIZE;
	GLuint num_tiles_height = resolution.y / Renderer_consts::TILESIZE;

	tiles.resize(num_tiles_height);
	for (GLuint i = 0; i < num_tiles_height; ++i){
	    tiles[i].resize(num_tiles_width);
	}

	return true;
}