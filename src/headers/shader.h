#ifndef SHADER_H
#define SHADER_H

/*Included headers*/
/*---------------------------------------------*/
#include "./errorlogger.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <unordered_map>
#include <assert.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

/* STATIC LINKAGE, CHANGE TO DYNAMIC LATER */
#define GLEW_STATIC

class Shader{
private:
	GLuint program;
public:
	Shader(const GLchar* vertex_shader_path, const GLchar* fragment_shader_path);
	char* load_from_file(const char* filename);
	GLuint create_shader(const char* filename, GLenum type);
	void print_log(GLuint object);
	void use(){glUseProgram(program);};
};
/*=============================================*/

#endif