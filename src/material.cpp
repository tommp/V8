#include "material.h"

Material::Material(){
	diffuse = nullptr;
	specular = nullptr;
	emissive = nullptr;
}

void Material::use(const Shader_ptr& shader){
	if (diffuse) {
		diffuse->use("material.diffuse", 0, shader);
	}
	if(specular){
		specular->use("material.specular", 1, shader);
	}
	if (emissive){
		emissive->use("material.emissive", 2, shader);
	}
	shininess = 32;
}

bool Material::load_from_file(Resource_manager& manager, const std::string& name){

	if (ENGINE_MATERIALS.find(name) == ENGINE_MATERIALS.end()) {
		std::cout << "ERROR: Material not found (missing in material map)!: " << name << std::endl;
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

	if (ENGINE_MATERIALS.find(name)->second[2] != "") {
		emissive = manager.load_texture(ENGINE_MATERIALS.find(name)->second[2]);
	}
	else{
		emissive = nullptr;
	}

	return true;
}