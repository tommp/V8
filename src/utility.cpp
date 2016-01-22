#include "headers/utility.h"

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
