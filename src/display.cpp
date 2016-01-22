#include "headers/display.h"

Display::Display(){
	/* Init to standard settings as a fallback */
	if (!load_settings()) {
		mouse_visible = false;
		use_vsync = true;
		use_fullscreen = false;
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
		screen_width = 0;
		screen_height = 0;

		errorlogger("ERROR: Failed to load display settings, restoring defaults.");
		std::cout << "ERROR: Failed to load display settings, restoring defaults." << std::endl;
		
		save_settings();
	}
	
	/* Initialize the window */
	init_window();

	/* Initialize opengl */
	init_openGL();

	/*Disables pesky screensavers while our wonderful graphics are beeing displayed*/
	SDL_DisableScreenSaver();
}

bool Display::save_settings(){
	std::ofstream contentf (DISPLAY_SETTINGS_FILE_PATH, std::ios::binary);

    if (!contentf.is_open()){
        errorlogger("ERROR: Failed to open content file for display settings!");
        std::cout << "ERROR: Failed to open content file for display settings!" << std::endl;
        return false;
    }

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
        errorlogger("ERROR: Failed to open content file for display settings!");
        std::cout << "ERROR: Failed to open content file for display settings!" << std::endl;
        return false;
    }

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
	window = SDL_CreateWindow("Cradlands", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (window == nullptr){
		SDLerrorLogger("SDL_CreateWindow");
		std::cout<<"ERROR: Failed to create SDL window, see errorlog for details."<<std::endl;
		return false;
	}

	/* Create opengl context */
    gl_context = SDL_GL_CreateContext(window);
    if(gl_context == NULL){
    	SDLerrorLogger("SDL_GL_CreateContext");
        std::cout << "ERROR: OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

	if(use_fullscreen){
		enable_fullscreen();
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
		SDLerrorLogger("SDL_GL_SetSwapInterval");
        std::cout << "ERROR: SDL_GL_SetSwapInterval could not enable vsync! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}
}

bool Display::disable_vsync(){
	if ( !(SDL_GL_SetSwapInterval(0) < 0)){
		return true;
	}
	else{
		SDLerrorLogger("SDL_GL_SetSwapInterval");
        std::cout << "ERROR: SDL_GL_SetSwapInterval could not disable vsync! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}
}

bool Display::init_openGL(){
	bool success = true;
	GLenum error = GL_NO_ERROR;

	/* Set this to true so GLEW knows to use a modern approach to retrieving 
	function pointers and extensions*/
    glewExperimental = GL_TRUE;

    /* Initialize GLEW to setup the OpenGL Function pointers */
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, width, height);

    /* Setup OpenGL options */
    glEnable(GL_DEPTH_TEST);

	/* Initialize Projection Matrix */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	error = glGetError();
	if(error != GL_NO_ERROR){
		std::cout <<"Error initializing OpenGL!" << error << std::endl;
		success = false;
	}

	/* Initialize Modelview Matrix */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	error = glGetError();
	if( error != GL_NO_ERROR ){
		std::cout <<"Error initializing OpenGL!" << error << std::endl;
		success = false;
	}

	/* Initialize clear color */
	glClearColor(0.f, 0.f, 0.f, 1.f);
	error = glGetError();
	if( error != GL_NO_ERROR ){
		std::cout <<"Error initializing OpenGL!" << error << std::endl;
		success = false;
	}
	return success;
}