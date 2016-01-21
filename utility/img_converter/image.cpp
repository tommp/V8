#include "./headers/image.h"

Image::Image() {
	id = 0;
	image = NULL;
	width = 0;
	height = 0;
	format = 0;
}

Image::Image(GLint init_id) {
	id = init_id;
	image = NULL;
	width = 0;
	height = 0;
	format = 0;
}

void Image::load_image(const char* path, GLuint image_id, GLuint image_format){
	id = image_id;
	format = image_format;
	image = SOIL_load_image(path, &width, &height, 0, format);
}

void Image::store_as_binary(char* registerfile, char* contentfile){
	std::ofstream registerf (registerfile, std::ios::binary | std::ios::ate | std::ios::app);
	std::ofstream contentf (contentfile, std::ios::binary | std::ios::ate | std::ios::app);

	GLuint size = contentf.tellp();

	registerf.write(reinterpret_cast<const char *>(&id), sizeof(GLuint));
	registerf.write(reinterpret_cast<const char *>(&size), sizeof(GLuint));
	registerf.write(reinterpret_cast<const char *>(&width), sizeof(GLuint));
	registerf.write(reinterpret_cast<const char *>(&height), sizeof(GLuint));

	contentf.write(reinterpret_cast<const char *>(&format), sizeof(GLint));
	contentf.write(reinterpret_cast<const char *>(image), sizeof(unsigned char)*width*height);

	registerf.close();
	contentf.close();
}

void Image::load_binary(char* registerfile, char* contentfile){
	std::ifstream registerf (registerfile, std::ios::binary);
	std::ifstream contentf (contentfile, std::ios::binary);

	GLuint datapos = -1;
	GLuint read_id;

	while(!registerf.eof()){
		registerf.read(reinterpret_cast<char *>(&read_id), sizeof(GLuint));
		if (read_id == id) {
			registerf.read(reinterpret_cast<char *>(&datapos), sizeof(GLuint));
			registerf.read(reinterpret_cast<char *>(&width), sizeof(GLuint));
			registerf.read(reinterpret_cast<char *>(&height), sizeof(GLuint));
			break;
		}
		else{
			registerf.seekg(sizeof(GLuint)*3, std::ios_base::cur);
		}
	}

	if (datapos >= 0){
		contentf.seekg(datapos);
		contentf.read(reinterpret_cast<char *>(&format), sizeof(GLint));
		contentf.read(reinterpret_cast<char *>(image), sizeof(unsigned char)*width*height);
	}
	else{
		std::cout << "FILEFAIL\n" << std::endl;
	}
	registerf.close();
	contentf.close();
}

void Image::save_image(std::string filename){
	SOIL_save_image(filename.c_str(),SOIL_SAVE_TYPE_BMP, width, height, 4, image);

}