#ifndef TEXTURE_H
#define TEXTURE_H


/*Included headers*/
/*---------------------------------------------*/
#include "./errorlogger.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <string>
#include <SDL2/SDL_image.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Texture
{
    private:
        /*The actual hardware texture*/
        SDL_Texture* texture;

        /*Image dimensions*/
        int width;
        int height;

    public:
        /*Initializes variables*/
        Texture();

        /*Deallocates memory*/
        ~Texture();

        /*Loads image at specified path*/
        bool load_from_file(SDL_Renderer *ren, std::string path);

        /*Deallocates texture*/
        void free();

        /*Renders texture at given point*/
        void render(SDL_Renderer *ren, int x, int y, SDL_Rect* clip = NULL);

        /*Gets image dimensions*/
        int get_width(){return width;};
        int get_height(){return height;};
};
/*=============================================*/
#endif