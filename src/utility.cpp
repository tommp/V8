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

bool load_binary_image(const std::string& name, unsigned char* image, GLuint* width, GLuint* height, GLint* format){
	std::ifstream contentf (IMAGE_DATA_PATH, std::ios::binary);

	if (!contentf.is_open()){
		errorlogger("ERROR: Failed to open content file for image data!");
		std::cout << "ERROR: Failed to open content file for image data!" << std::endl;
		return false;
	}

	if (WORLD_IMAGES.find(name) == WORLD_IMAGES.end()){
		errorlogger("ERROR: No image in image_map with keyname: ", name.c_str());
		std::cout << "ERROR: No image in image_map with keyname: " << name.c_str() << std::endl;
		return false;
	}

	GLuint datapos = WORLD_IMAGES.find(name)->second;

	contentf.seekg(datapos);
	contentf.read(reinterpret_cast<char *>(width), sizeof(GLuint));
	contentf.read(reinterpret_cast<char *>(height), sizeof(GLuint));
	contentf.read(reinterpret_cast<char *>(format), sizeof(GLint));
	contentf.read(reinterpret_cast<char *>(image), sizeof(unsigned char)*(*width)*(*height));

	contentf.close();

	return true;
}

bool load_binary_mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<GLuint>& indices){
	std::ifstream contentf (MESH_DATA_PATH, std::ios::binary);

	if (!contentf.is_open()){
		errorlogger("ERROR: Failed to open content file for mesh data!");
		std::cout << "ERROR: Failed to open content file for mesh data!" << std::endl;
		return false;
	}

	if (WORLD_MESHES.find(name) == WORLD_MESHES.end()){
		errorlogger("ERROR: No image in mesh_map with keyname: ", name.c_str());
		std::cout << "ERROR: No image in mesh_map with keyname: " << name.c_str() << std::endl;
		return false;
	}

	GLuint datapos = WORLD_MESHES.find(name)->second.first;

	contentf.seekg(datapos);

	/* LOAD VERTICES AND INDICES FROM FILE HERE */

	/* ======================================== */

	contentf.close();

	return true;
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