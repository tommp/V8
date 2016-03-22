#ifndef SHADER_H
#define SHADER_H

/*Included headers*/
/*---------------------------------------------*/
#include "paths.h"
#include "glm.h"
#include "errorlogger.h"
#include "shader_map.h"
#include "utility.h"
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
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

/* STATIC LINKAGE, CHANGE TO DYNAMIC LATER */
#define GLEW_STATIC
#define OPENGL_VERSION "#version 330 core\n"

class Shader{
private:
	GLuint program;
	std::unordered_map<std::string, GLuint> uniform_locations;
public:
	Shader();
	GLuint get_program(){return program;};
	GLuint load_uniform_location(const std::string& uniform);
	char* read_data_from_file(const char* filename);
	bool load_from_file(const std::string& name);
	GLuint create_shader(const char* filename, GLenum type);
	void print_log(GLuint object);
	void use(){glUseProgram(program);};
};

typedef std::shared_ptr<Shader> Shader_ptr;
typedef std::weak_ptr<Shader> Shader_weak;
/*=============================================*/

#endif