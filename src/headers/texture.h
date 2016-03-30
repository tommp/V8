#ifndef TEXTURE_H
#define TEXTURE_H


/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
#include "utility.h"
#include "shader.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <GL/glew.h>
#include <string>
#include <memory>
#include <cstdlib>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Texture
{
    private:
        std::string name;
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
        bool free_texture();
       bool use(const std::string& uniform_name, GLuint texture_unit, const Shader_ptr& shader);

        bool load_from_file(const std::string& name);
        unsigned char* load_binary_texture(const std::string& name);

        int get_width()const{return width;};
        int get_height()const{return height;};
};

typedef std::shared_ptr<Texture> Texture_ptr;
/*=============================================*/
#endif