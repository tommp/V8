#include "display.h"

Display::Display(){
	/* Init to standard settings as a fallback */
	if (!load_settings()) {
		ortographic = true;
		mouse_visible = false;
		use_vsync = true;
		use_fullscreen = false;
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
		screen_width = SCREEN_WIDTH;
		screen_height = SCREEN_HEIGHT;

		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to load display settings, restoring defaults." << std::endl;
		errorlogger("ERROR: Failed to load display settings, restoring defaults.");

		save_settings();
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

	update_projection_matrix();

	/*Disables pesky screensavers while our wonderful graphics are beeing displayed*/
	SDL_DisableScreenSaver();
	clear();
}

void Display::update_projection_matrix(){
	if (ortographic) {
		projection = glm::ortho(0.0f, (GLfloat)width, 0.0f, (GLfloat)height, 0.1f, 6000.0f);
	}
	else{
		projection = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 3000.0f);
	}
}

void Display::upload_projection_matrix(GLuint matrix_uniform_buffer){
	glBindBuffer(GL_UNIFORM_BUFFER, matrix_uniform_buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);  

	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload projection matix!" << std::endl;
		errorlogger("ERROR: Failed to upload projection matix!");
		exit(EXIT_FAILURE);
	}
}

bool Display::save_settings(){
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

	contentf.write(reinterpret_cast<char *>(&width), sizeof(GLint));
	contentf.write(reinterpret_cast<char *>(&height), sizeof(GLint));
	contentf.write(reinterpret_cast<char *>(&screen_height), sizeof(GLint));
	contentf.write(reinterpret_cast<char *>(&screen_width), sizeof(GLint));

	contentf.close();

	return true;
}

bool Display::load_settings(){
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

	contentf.read(reinterpret_cast<char *>(&width), sizeof(GLuint));
	contentf.read(reinterpret_cast<char *>(&height), sizeof(GLuint));
	contentf.read(reinterpret_cast<char *>(&screen_height), sizeof(GLuint));
	contentf.read(reinterpret_cast<char *>(&screen_width), sizeof(GLuint));

	contentf.close();

	return true;
}

void Display::toggle_mouse(){
	if (mouse_visible) {
		SDL_ShowCursor(0);
	}
	else{
		SDL_ShowCursor(1);
	}
}

bool Display::init_window(){

	/* Set to enable opengl window context */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);

	/*Initializes a window to render graphics in*/
	window = SDL_CreateWindow("V8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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

bool Display::enable_fullscreen(){
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_GetWindowSize(window, &screen_width, &screen_height);
	return true;
}

bool Display::enable_vsync(){
	if ( !(SDL_GL_SetSwapInterval(1) < 0)){
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: SDL_GL_SetSwapInterval could not enable vsync! SDL Error: " << SDL_GetError() << std::endl;
		SDLerrorLogger("SDL_GL_SetSwapInterval");
		return false;
	}
}

bool Display::disable_vsync(){
	if ( !(SDL_GL_SetSwapInterval(0) < 0)){
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: SDL_GL_SetSwapInterval could not disable vsync! SDL Error: " << SDL_GetError() << std::endl;
		SDLerrorLogger("SDL_GL_SetSwapInterval");
		return false;
	}
}

bool Display::init_openGL(){
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