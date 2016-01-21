#include "./headers/texture.h"

Texture::Texture(){
	/* Initialize to zero */
	glGenTextures(1, &texture);
	width = 0;
	height = 0;
}

Texture::~Texture(){
	/* Deallocate */
    free();
}

void Texture::free(){
    /* Free texture if it exists */
    if( texture != NULL )
    {
        glDeleteTextures(1, &texture);
        texture = 0;
        width = 0;
        height = 0;
    }
}

bool Texture::load_from_file(const std::string& name){
	/* Get rid of preexisting texture */
	if( texture != NULL ){
    	free();
	}

    unsigned char* image;
    if (!load_binary_image(name, image, &width, &height, &format)) {
        errorlog("ERROR: Error propogation from load_binary_image(..) when loading keyname: ", name.c_str())
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
	
    if( texture == NULL ){
    	errorLogger("ERROR: Unable to create texture from keyname: ", name.c_str());
        std::cout << "ERROR: Unable to create texture from keyname: "<< name.c_str() << std::endl;
        return false;
    }

    return true;
}

void Texture::render(SDL_Renderer& ren, int x, int y, SDL_Rect* clip){
	/* Set rendering space and render to screen */
    SDL_Rect renderQuad = { x, y, width, height };

    /* Set clip rendering dimensions */
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    /* Render to screen */
    SDL_RenderCopy(&ren, texture, clip, &renderQuad );
}