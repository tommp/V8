#include "material.h"

Material::Material(){
	diffuse = nullptr;
	specular = nullptr;
	shader = nullptr;
}

void Material::use(){
	if (diffuse) {
		diffuse->use("material.diffuse", 0, shader);
	}
	if(specular){
		specular->use("material.specular", 1, shader);
	}
	shininess = 32;
}

bool Material::load_from_file(Resource_manager& manager, const std::string& name){

	if (ENGINE_MATERIALS.find(name) == ENGINE_MATERIALS.end()) {
		std::cout << "ERROR: Material not found (missing in material map)!: " << name << std::endl;
		errorlogger("ERROR: Material not found (missing in material map)!: ", name.c_str());
		return false;
	}

	if (ENGINE_MATERIALS.find(name)->second.first != "") {
		shader = manager.load_shader(ENGINE_MATERIALS.find(name)->second.first);
	}
	else{
		shader = nullptr;
	}

	if (ENGINE_MATERIALS.find(name)->second.second[0] != "") {
		diffuse = manager.load_texture(ENGINE_MATERIALS.find(name)->second.second[0]);
	}
	else{
		diffuse = nullptr;
	}

	if (ENGINE_MATERIALS.find(name)->second.second[1] != "") {
		specular = manager.load_texture(ENGINE_MATERIALS.find(name)->second.second[1]);
	}
	else{
		specular = nullptr;
	}

	return true;
}