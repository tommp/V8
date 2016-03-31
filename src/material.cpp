#include "material.h"

Material::Material(){
	diffuse = nullptr;
	specular = nullptr;
}

bool Material::use(const Shader_ptr& shader){
	if (diffuse) {
		if (!diffuse->use("material.diffuse", 3, shader)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to use diffuse texture for material: " << name << std::endl;
			errorlogger("ERROR: Failed to use diffuse texture for material: ", name.c_str());
			return false;
		}
	}
	if(specular){
		if (!specular->use("material.specular", 4, shader)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to use specular texture for material: " << name << std::endl;
			errorlogger("ERROR: Failed to use specular texture for material: ", name.c_str());
			return false;
		}
	}
	shininess = 32;

	return true;
}

bool Material::load_from_file(Resource_manager& manager, const std::string& name){
	std::string material_path = Utility_vars::folder_path + MATERIAL_DATA_PATH + name + ".mat";

	this->name = name;

	std::ifstream contentf (material_path.c_str(), std::ios::binary);
	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for material data: " << material_path << std::endl;
		errorlogger("ERROR: Failed to open content file for material data: ", material_path.c_str());
		return false;
	}

	GLuint has_diffuse;
	GLuint has_specular;

	contentf.read(reinterpret_cast<char *>(&has_diffuse), sizeof(GLuint));

	if (has_diffuse != 0) {
		std::string diffuse_name;
		if (!read_string_from_binary_file(contentf, diffuse_name)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Failed to read diffuse name for material from file: " << material_path << std::endl;
			diffuse = manager.load_texture("engine_base_diffuse");
		}
		else{
			diffuse = manager.load_texture(diffuse_name);
		}
	}
	else{
		std::cout << "No diffuse texture, using base" << std::endl;
		diffuse = manager.load_texture("engine_base_diffuse");
	}

	contentf.read(reinterpret_cast<char *>(&has_specular), sizeof(GLuint));

	if (has_specular != 0){
		std::string specular_name;
		if (!read_string_from_binary_file(contentf, specular_name)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Failed to read specular name for material from file: " << material_path << std::endl;
			specular = manager.load_texture("engine_base_specular");
		}
		else{
			specular = manager.load_texture(specular_name);
		}
	}
	else{
		std::cout << "No specular texture, using base" << std::endl;
		diffuse = manager.load_texture("engine_base_specular");
	}

	return true;
}

bool Material::is_complete()const {
	if (diffuse && specular) {
		return true;
	}
	
	return false;
}