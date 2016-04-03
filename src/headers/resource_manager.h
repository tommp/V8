#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "enum_key_type.h"
#include "button_mappings.h"
#include "shader.h"
#include "texture.h"
#include "material.h"
#include "mesh.h"
#include "model.h"
#include "animation.h"
#include "animation_set.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <unistd.h>
#include <unordered_map>
#include <array>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Animation;
typedef std::shared_ptr<Animation> Animation_ptr;
class Animation_set;
typedef std::shared_ptr<Animation_set> Animation_set_ptr;
class Mesh;
typedef std::shared_ptr<Mesh> Mesh_ptr;
class Model;
typedef std::shared_ptr<Model> Model_ptr;
class Material;
typedef std::shared_ptr<Material> Material_ptr;
class Shader;
typedef std::shared_ptr<Shader> Shader_ptr;
class Button_mappings;

class Resource_manager {
	private:

		std::unordered_map<std::string, Button_mappings> button_mappings;

		std::unordered_map<std::string, Texture_ptr> textures;
		std::unordered_map<std::string, Material_ptr> materials;
		std::unordered_map<std::string, Animation_ptr> animations;
		std::unordered_map<std::string, Animation_set_ptr> animation_sets;
		std::unordered_map<std::string, Mesh_ptr> meshes;
		std::unordered_map<std::string, Model_ptr> models;
		std::unordered_map<std::string, Shader_ptr> shaders;
	public:
		Resource_manager();

		SDL_Keycode get_button_map_key(const std::string& map_name, const Key& key)const;

		Texture_ptr load_texture(const std::string& name);
		Material_ptr load_material(const std::string& name);
		Mesh_ptr load_mesh(const std::string& name);
		Mesh_ptr load_mesh(const std::string& name, const glm::vec4& color);
		Model_ptr load_model(const std::string& name);
		Model_ptr load_model(const std::string& name, const glm::vec4& color);
		Shader_ptr load_shader(const std::string& name);
		Animation_ptr load_animation(const std::string& name);
		Animation_set_ptr load_animation_set(const std::string& name);
};
/*=============================================*/

#endif

