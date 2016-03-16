#ifndef MESH_H
#define MESH_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "bone.h"
#include "vertex.h"
#include "rendering_context.h"
#include "utility.h"
#include "errorlogger.h"
#include "renderer.h"
#include "material.h"
#include "resource_manager.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <GL/glew.h>
#include <string>
#include <unistd.h>
#include <vector>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

/* TODO:: Hack, fix by passing mesh init dir later */
const glm::vec3 MESH_DIRECTION = {0.0f, 1.0f, -1.0f};

class Resource_manager;
class Material;
class Renderer;

typedef std::shared_ptr<Material> Material_ptr;

class Mesh {
	private:
		Rendering_context_ptr rendering_context;

		std::string name;
		glm::vec3 init_direction;
		GLuint VBO;
		GLuint EBO;

		/* Inverse transform of the root bone transformation used in animation */
		glm::mat4 root_inverse_transform;
		
		std::unordered_map<GLuint, GLuint> bone_map;
		std::vector<glm::mat4> bone_info;

		/* Used for optimization */
		glm::vec3 prev_position;
		glm::vec3 prev_size;
		glm::vec3 prev_direction;
	public:
		Mesh();
		~Mesh();
		void free_mesh();
		bool load_binary_mesh(const std::string& name, 
							std::vector<Vertex>& vertices, 
							std::vector<GLuint>& indices,
							std::string& material);

		bool load_from_file(Resource_manager& manager, const std::string& name);
		bool add_context_to_renderer(Renderer& renderer)const;
		bool update_context(const glm::vec3& position, 
							const glm::vec3& size, 
							const glm::vec3& direction);
};

typedef std::shared_ptr<Mesh> Mesh_ptr;
/*=============================================*/

#endif