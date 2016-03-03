#include "utility.h"

std::vector<std::string> glob(const std::string& path){
    glob_t glob_result;
    glob(path.c_str(), GLOB_TILDE, NULL, &glob_result);
    std::vector<std::string> ret;
    for(unsigned int i=0; i<glob_result.gl_pathc; i++){
        ret.push_back(std::string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return ret;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
    	if (!item.empty()){
    		elems.push_back(item);
    	}
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

bool convert_all_models(){
	return true;
}

bool convert_model_file(){
	return true;
}

bool store_binary_model(){
	return true;
}

bool store_binary_mesh(){
	return true;
}

bool convert_all_images(){
	std::string source_path_mask = IMAGE_DATA_PATH;
	source_path_mask += "*";
	std::vector<std::string> sources = glob(source_path_mask.c_str());
	std::vector<std::string> targets;
	for (auto path : sources) {
		std::vector<std::string> tokens = split(path, '/');
		std::string filename = tokens.back();
		std::vector<std::string> file_tokens = split(filename, '.');
		std::string new_target = (TEXTURE_DATA_PATH + file_tokens[0] + ".tex");
		targets.push_back(new_target);
	}

	if(sources.size() != targets.size()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to convert images, size error in vectors!" << std::endl;
		errorlogger("ERROR: Failed to convert images, size error in vectors!");
		return false;
	}
	
	for(GLuint i = 0; i < sources.size(); ++i) {
		if (!convert_image_file(sources[i], targets[i])){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to convert image: " << sources[i] << std::endl;
			errorlogger("ERROR: Failed to convert image: ", sources[i].c_str());
		}
	}
	return true;
}

bool convert_image_file(const std::string& source_path, const std::string& target_path){
	GLint width, height, channels;
	unsigned char* image = SOIL_load_image(source_path.c_str(), &width, &height, &channels, SOIL_LOAD_RGB);
	if (!image) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: SOIL failed to convert image: " << source_path << std::endl;
		errorlogger("ERROR: SOIL failed to convert image: ", source_path.c_str());
		return false;
	}

	if (!store_binary_texture(target_path.c_str(), image, width, height, channels, SOIL_LOAD_RGB)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to store target texture: " << target_path << std::endl;
		errorlogger("ERROR: Failed to store target texture: ", target_path.c_str());
	}

	return true;
}

bool store_binary_texture(const std::string& path, 
							unsigned char* image, 
							GLuint width, 
							GLuint height, 
							GLuint channels, 
							GLint format){
	std::ofstream contentf (path.c_str(), std::ios::binary);
	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for storing texture data!" << std::endl;
		errorlogger("ERROR: Failed to open content file for storing texture data!");
		return false;
	}

	contentf.write(reinterpret_cast<const char *>(&width), sizeof(GLuint));
	contentf.write(reinterpret_cast<const char *>(&height), sizeof(GLuint));
	contentf.write(reinterpret_cast<const char *>(&channels), sizeof(GLuint));
	contentf.write(reinterpret_cast<const char *>(&format), sizeof(GLint));
	contentf.write(reinterpret_cast<const char *>(&(image[0])), sizeof(unsigned char) * width * height * channels);

	contentf.close();

	return true;
}

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