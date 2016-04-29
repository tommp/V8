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
#include "shader.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <GL/glew.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

namespace Mesh_vars{
	const std::string BOX = "BOX";
	const std::string LINE = "LINE";
	const std::string SPHERE = "SPHERE";
};

class Resource_manager;
class Material;
class Renderer;

typedef std::shared_ptr<Material> Material_ptr;

class Mesh {
	private:
		Rendering_context_ptr base_context;

		GLboolean mesh_in_renderer;

		std::string name;
		GLuint VBO;
		GLuint EBO;

		/* Uniforms */
		Material_ptr material;
		glm::vec4 object_color;

		/* Inverse transform of the root bone transformation used in animation */
		glm::mat4 root_inverse_transform;
		
		std::unordered_map<GLuint, GLuint> bone_map;
		std::vector<glm::mat4> bone_info;

		bool free_mesh();
		bool load_binary_mesh(const std::string& name, 
							std::vector<Vertex>& vertices, 
							std::vector<GLuint>& indices,
							std::string& material);

		bool load_base_box(std::vector<Vertex>& vertices);
		bool load_base_line(std::vector<Vertex>& vertices);
	public:
		Mesh();
		Mesh(const glm::vec4& color);
		~Mesh();
		
		bool load_from_file(Resource_manager& manager, const std::string& name);
		Rendering_context_weak get_context()const;
		Rendering_context_ptr get_main_context()const;
		bool setup_base_uniforms(const Shader_ptr& shader);

		bool add_context_to_renderer(Renderer& renderer);
		bool add_uniform_setup(const std::string& modelname, 
					std::function<GLboolean(const Shader_ptr& shader, GLuint instance, GLboolean only_model)> expression);
		bool remove_uniform_setup(const std::string& modelname);
		std::string& get_name(){return name;};
};

typedef std::shared_ptr<Mesh> Mesh_ptr;
/*=============================================*/

#endif