#include "model.h"

Model::Model(){
}

void Model::render_model(const glm::vec3& position, const glm::vec3& size, const glm::vec3& direction)const{
	for (auto mesh : meshes) {
		mesh->render_mesh(position, size, direction);
	}
}

bool Model::load_from_file(Resource_manager& manager, const std::string& name){

	if (ENGINE_MODELS.find(name) == ENGINE_MODELS.end()) {
		errorlogger("ERROR: Model not found!: ", name.c_str());
		std::cout << "ERROR: Model not found!: " << name << std::endl;
		return false;
	}

	for (auto mesh : ENGINE_MODELS.find(name)->second) {
		Mesh_ptr new_mesh = manager.load_mesh(mesh);
		if (!new_mesh) {
			errorlogger("Unable to load mesh in model from resource handler: ", mesh.c_str());
			std::cout << "Unable to load mesh in model from resource handler: " << mesh << std::endl;
			return false;
		}
		meshes.push_back(new_mesh);
	}
	return true;
}