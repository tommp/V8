#include "./headers/texture.h"

Texture::Texture(){
	/* Initialize to zero */
	glGenTextures(1, &texture);
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

bool Texture::load_from_file(const std::string& name){
	/* Get rid of preexisting texture */
	if( texture != 0 ){
    	free_texture();
	}

    unsigned char* image = nullptr;
    if (!load_binary_image(name, image, &width, &height, &format)) {
        errorlogger("ERROR: Error propogation from load_binary_image(..) when loading keyname: ", name.c_str());
        std::cout << "ERROR: Error propogation from load_binary_image(..) when loading keyname: " << name.c_str() << std::endl;
        return false;
    }

    /* Create and bind texture from image data */
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    free(image);
	
    if( texture == 0 ){
    	errorlogger("ERROR: Unable to create texture from keyname: ", name.c_str());
        std::cout << "ERROR: Unable to create texture from keyname: "<< name.c_str() << std::endl;
        return false;
    }

    return true;
}