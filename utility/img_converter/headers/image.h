#ifndef IMAGE_H
#define IMAGE_H


/*Included headers*/
/*---------------------------------------------*/
#include "./SOIL.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>
#include <unistd.h>
#include <fstream>
#include <iostream>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Image{
private:
	GLuint id;
	unsigned char* image;
	GLint width;
	GLint height;
	GLint format;

public:
	Image();
	Image(GLint init_id);
	void load_image(const char* path, GLuint image_id, GLuint image_format); //Fortmat SOIL_LOAD_RGBA
	void store_as_binary(char* registerfile, char* contentfile);
	void load_binary(char* registerfile, char* contentfile);
};
/*=============================================*/

#endif