#include "./headers/texture.h"

Texture::Texture(){
	/* Initialize to zero */
	texture = NULL;
	width = 0;
	height = 0;
}

Texture::~Texture(){
	/* Deallocate */
    free();
}

bool Texture::load_from_file( SDL_Renderer& ren, const std::string& name ){
	/* Get rid of preexisting texture */
	if( texture != NULL ){
    	free();
	}

	/* The final texture */
    SDL_Texture* newTexture = NULL;

    if (WORLD_TEXTURES.find(name) == WORLD_TEXTURES.end()) {
        std::cout << "ERROR: Texture not found!: " << name << std::endl;
        errorlogger("ERROR: Texture not found!: ", name.c_str());
        return false;
    }

    std::string path = WORLD_TEXTURES.find(name)->second;

	/* Create texture from path */
    newTexture = IMG_LoadTexture( &ren, path.c_str() );;
    if( newTexture == NULL ){
    	SDLerrorLogger( "Unable to create texture!" );
        printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }
    else{
        /* Get image dimensions */
        SDL_QueryTexture(newTexture, NULL, NULL, &width, &height);
    }

    /* Return */
    texture = newTexture;
    return ( texture != NULL );
}

void Texture::free(){
	/* Free texture if it exists */
    if( texture != NULL )
    {
        SDL_DestroyTexture( texture );
        texture = NULL;
        width = 0;
        height = 0;
    }
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