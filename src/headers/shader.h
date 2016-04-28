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

namespace Shader_vars{
	extern std::string opengl_version;
}

class Shader{
private:
	std::string name;
	GLuint program;
	std::unordered_map<std::string, GLuint> uniform_locations;
	bool bind_uniform_buffers(GLuint program, const std::string& name)const;
	char* read_data_from_file(const char* filename);
public:
	Shader();
	GLuint get_program(){return program;};
	GLuint load_uniform_location(const std::string& uniform);
	GLuint load_uniform_location(const std::string& uniform, GLuint uniform_index);
	GLuint load_uniform_location(const std::string& uniform, GLuint uniform_index, const std::string& uniform_member);
	
	bool load_from_file(const std::string& name);
	GLuint create_shader(const char* filename, GLenum type);
	void print_log(GLuint object);
	bool use();
};

typedef std::shared_ptr<Shader> Shader_ptr;
typedef std::weak_ptr<Shader> Shader_weak;
/*=============================================*/

#endif