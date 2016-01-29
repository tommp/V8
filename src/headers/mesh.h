#ifndef MESH_H
#define MESH_H


/*Included headers*/
/*---------------------------------------------*/
#include "vertex.h"
#include "shader.h"
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "texture.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <unistd.h>
#include <vector>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
using std::vector;

class Mesh {
	private:
		GLuint VBO;
		GLuint VAO;
		GLuint EBO;

		GLuint num_vertices;

		Shader_ptr shader;
	public:
		Mesh();
		Mesh(Shader_ptr init_shader);
		bool load_from_file(const std::string& name);
		void free_mesh();
		void render_mesh(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, GLfloat rotate);
};

typedef std::shared_ptr<Mesh> Mesh_ptr;
/*=============================================*/

#endif