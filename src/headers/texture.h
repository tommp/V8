#ifndef TEXTURE_H
#define TEXTURE_H


/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
#include "utility.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>
#include <SDL2/SDL_image.h>
#include <memory>
#include <cstdlib>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Texture
{
    private:
        /*The actual hardware texture*/
        GLuint texture;

        /*Image dimensions*/
        GLuint width;
        GLuint height;

        /* Image format */
        GLint format;
    public:
        /*Initializes variables*/
        Texture();

        /*Deallocates memory*/
        ~Texture();

        /*Deallocates texture*/
        void free_texture();

        /*Loads image at specified path*/
        bool load_from_file(const std::string& name);

        /*Gets image dimensions*/
        int get_width()const{return width;};
        int get_height()const{return height;};
};

typedef std::shared_ptr<Texture> Texture_ptr;
/*=============================================*/
#endif