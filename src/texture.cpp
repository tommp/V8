#include "texture.h"

Texture::Texture(){
	/* Initialize */
	width = 0;
	height = 0;
}

Texture::~Texture(){
	/* Deallocate */
	free_texture();
}

void Texture::free_texture(){
	/* Free texture if it exists */
	if( texture != 0 )
	{
		glDeleteTextures(1, &texture);
		texture = 0;
		width = 0;
		height = 0;
	}
}

void Texture::use(const std::string& uniform_name, GLuint texture_unit, const Shader_ptr& shader){
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shader->get_program(), uniform_name.c_str()), texture_unit);
    if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind texture when using: " << uniform_name << ", with texture: " << name << std::endl;
		errorlogger("ERROR: Failed to bind texture when using: ", uniform_name.c_str());
	}
}

unsigned char* Texture::load_binary_image(const std::string& name){
	std::ifstream contentf (IMAGE_DATA_PATH, std::ios::binary);

	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for image data!" << std::endl;
		errorlogger("ERROR: Failed to open content file for image data!");
		return nullptr;
	}

	if (ENGINE_TEXTURES.find(name) == ENGINE_TEXTURES.end()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: No image in image_map with keyname: " << name << std::endl;
		errorlogger("ERROR: No image in image_map with keyname: ", name.c_str());
		return nullptr;
	}

	GLuint datapos = ENGINE_TEXTURES.find(name)->second;

	contentf.seekg(datapos);
	contentf.read(reinterpret_cast<char *>(&width), sizeof(GLuint));
	contentf.read(reinterpret_cast<char *>(&height), sizeof(GLuint));
	contentf.read(reinterpret_cast<char *>(&channels), sizeof(GLuint));
	contentf.read(reinterpret_cast<char *>(&format), sizeof(GLint));

	/* DANGER: Fix this later */
	unsigned char* image = new unsigned char[width * height * channels];

	contentf.read(reinterpret_cast<char *>(image), sizeof(unsigned char) * width * height * channels);
	contentf.close();

	return image;
}

bool Texture::load_from_file(const std::string& name){
	/* Get rid of preexisting texture */
	if( texture != 0 ){
		free_texture();
	}

	this->name = name;
	unsigned char* image_data = load_binary_image(name);
	
	if (!image_data) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Invalid image data for texture when loading keyname: " << name.c_str() << std::endl;
		errorlogger("ERROR: Invalid image data for texture when loading keyname: ", name.c_str());
		return false;
	}
	/* Create and bind texture from image data */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); 
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	//glGenerateMipmap(GL_TEXTURE_2D);/* TODO: Remove this if using ortographic projection */
	glBindTexture(GL_TEXTURE_2D, 0);

	delete[](image_data);

	if(check_ogl_error()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to create and bind texture from image data! Keyname: " << name << std::endl;
		errorlogger("ERROR: Failed to create and bind texture from image data! Keyname: ", name.c_str());
		return false;
	}
	return true;
}