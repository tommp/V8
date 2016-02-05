#include "utility.h"

void waitForEvent(){
	SDL_Event event;
	bool done = false;
	while((!done) && (SDL_WaitEvent(&event))) {
		switch(event.type) {
	
			case SDL_KEYDOWN:
				done = true;
				break;

			case SDL_QUIT:
				done = true;
				break;
				
			default:
				break;
		} 
			
	}
}

const char* gl_error_string(GLenum err){
  	switch(err) {
		case GL_INVALID_ENUM: return "Invalid Enum";
		case GL_INVALID_VALUE: return "Invalid Value";
		case GL_INVALID_OPERATION: return "Invalid Operation";
		case GL_STACK_OVERFLOW: return "Stack Overflow";
		case GL_STACK_UNDERFLOW: return "Stack Underflow";
		case GL_OUT_OF_MEMORY: return "Out of Memory";
		case GL_TABLE_TOO_LARGE: return "Table too Large";
		default: return "Unknown Error";
	}
}

int check_ogl_error(){
	GLenum gl_error;
	int retCode = 0;

	gl_error = glGetError();

	while (gl_error != GL_NO_ERROR){
		errorlogger("ERROR: glError in file: ", __FILE__);
		std::cout << "ERROR: glError in file: " << __FILE__ << ", line:" << __LINE__ << ", errorcode: " << gl_error_string(gl_error) << std::endl;
		gl_error = glGetError();
		retCode = 1;
	}

	return retCode;
}