#ifndef TEXTURE_H
#define TEXTURE_H


/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
#include "utility.h"
#include "shader.h"
#include "texture_map.h"
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
        GLuint channels;

        /* Image format */
        GLint format;
    public:
        Texture();
        ~Texture();
        void free_texture();
        void use(const std::string& uniform_name, GLuint texture_unit, const Shader_ptr& shader);

        bool load_from_file(const std::string& name);
        unsigned char* load_binary_image(const std::string& name);

        int get_width()const{return width;};
        int get_height()const{return height;};
};

typedef std::shared_ptr<Texture> Texture_ptr;
/*=============================================*/
#endif