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

void print_framebuffer_error_in_fucking_english(){
	switch(glCheckFramebufferStatus(GL_FRAMEBUFFER)){
		case GL_FRAMEBUFFER_UNDEFINED:
			std::cout <<  "FRAMEBUFFER ERROR: Default framebuffer does not exist." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout <<  "FRAMEBUFFER ERROR:  Framebuffer attachment point(s) are framebuffer incomplete." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout <<  "FRAMEBUFFER ERROR: Missing attachment." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			std::cout <<  "FRAMEBUFFER ERROR: Incomplete draw buffer." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			std::cout <<  "FRAMEBUFFER ERROR: Incomplete read buffer." << std::endl;
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout <<  "FRAMEBUFFER ERROR: Combination of internal formats of the attached images violates an implementation-dependent set of restrictions." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			std::cout <<  "FRAMEBUFFER ERROR: Invalid values for samples." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			std::cout <<  "FRAMEBUFFER ERROR: Incomplete layers." << std::endl;
			break;
		default:
			std::cout <<  "FRAMEBUFFER ERROR: Unknown error!" << std::endl;
			break;
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
		errorlogger("ERROR: glError: ", gl_error_string(gl_error));
		std::cout << "ERROR: glError: " << gl_error_string(gl_error) << std::endl;
		gl_error = glGetError();
		retCode = 1;
	}

	return retCode;
}