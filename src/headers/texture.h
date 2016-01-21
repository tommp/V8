#ifndef TEXTURE_H
#define TEXTURE_H


/*Included headers*/
/*---------------------------------------------*/
#include "./errorlogger.h"
#include "./utility.h"
#include "./SOIL.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <string>
#include <SDL2/SDL_image.h>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Texture
{
    private:
        /*The actual hardware texture*/
        GLuint texture;

        /*Image dimensions*/
        int width;
        int height;

    public:
        /*Initializes variables*/
        Texture();

        /*Deallocates memory*/
        ~Texture();

        /*Loads image at specified path*/
        bool load_from_file(SDL_Renderer& ren, const std::string& name);

        /*Deallocates texture*/
        void free();

        /*Renders texture at given point*/
        void render(SDL_Renderer& ren, int x, int y, SDL_Rect* clip = NULL);

        /*Gets image dimensions*/
        int get_width()const{return width;};
        int get_height()const{return height;};
};

typedef std::shared_ptr<Texture> Texture_ptr;
/*=============================================*/
#endif