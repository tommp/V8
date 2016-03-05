#include "material.h"

Material::Material(){
	diffuse = nullptr;
	specular = nullptr;
}

void Material::use(const Shader_ptr& shader){
	if (diffuse) {
		diffuse->use("material.diffuse", 3, shader);
	}
	if(specular){
		specular->use("material.specular", 4, shader);
	}
	shininess = 32;
}

bool Material::load_from_file(Resource_manager& manager, const std::string& name){
	std::string model_path = MATERIAL_DATA_PATH + name + ".mat";

	std::ifstream contentf (model_path.c_str(), std::ios::binary);
	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for material data: " << model_path << std::endl;
		errorlogger("ERROR: Failed to open content file for material data: ", model_path.c_str());
		return false;
	}

	GLuint has_diffuse;
	GLuint has_specular;

	contentf.read(reinterpret_cast<char *>(&has_diffuse), sizeof(GLuint));

	if (has_diffuse != 0) {
		diffuse = manager.load_texture(read_string_from_binary_file(contentf));
	}
	else{
		std::cout << "No diffuse texture, using base" << std::endl;
		diffuse = manager.load_texture("engine_base_diffuse");
	}

	contentf.read(reinterpret_cast<char *>(&has_specular), sizeof(GLuint));

	if (has_specular != 0){
		specular = manager.load_texture(read_string_from_binary_file(contentf));
	}
	else{
		std::cout << "No specular texture, using base" << std::endl;
		diffuse = manager.load_texture("engine_base_specular");
	}

	return true;
}