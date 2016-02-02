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
#include "resource_manager.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>
#include <unistd.h>
#include <vector>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
using std::vector;

const glm::vec3 MESH_DIRECTION = {0.0f, 0.0f, -1.0f};

class Resource_manager;

class Mesh {
	private:
		GLuint VBO;
		GLuint VAO;
		GLuint EBO;

		GLuint num_vertices;
	public:
		Mesh();
		~Mesh();
		void free_mesh();
		bool load_from_file(const Resource_manager& resource_manager, const std::string& name);
		void render_mesh(const Shader_ptr& shader, const glm::vec3& position, const glm::vec3& size, const glm::vec3& direction);
};

typedef std::shared_ptr<Mesh> Mesh_ptr;
/*=============================================*/

#endif