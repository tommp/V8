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

	if (ENGINE_MATERIALS.find(name) == ENGINE_MATERIALS.end()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Material not found (missing in material map)!: " << name << std::endl;
		errorlogger("ERROR: Material not found (missing in material map)!: ", name.c_str());
		return false;
	}

	if (ENGINE_MATERIALS.find(name)->second[0] != "") {
		diffuse = manager.load_texture(ENGINE_MATERIALS.find(name)->second[0]);
	}
	else{
		diffuse = nullptr;
	}

	if (ENGINE_MATERIALS.find(name)->second[1] != "") {
		specular = manager.load_texture(ENGINE_MATERIALS.find(name)->second[1]);
	}
	else{
		specular = nullptr;
	}

	return true;
}