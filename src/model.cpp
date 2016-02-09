#include "model.h"

Model::Model(){
}

void Model::render_model(const Renderer& renderer, const glm::vec3& position, const glm::vec3& size, const glm::vec3& direction)const{
	for (auto mesh : meshes) {
		mesh->render_mesh(renderer, position, size, direction);
	}
}

bool Model::load_from_file(Resource_manager& manager, const std::string& name){

	if (ENGINE_MODELS.find(name) == ENGINE_MODELS.end()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Model not found!: " << name << std::endl;
		errorlogger("ERROR: Model not found!: ", name.c_str());
		return false;
	}

	for (auto mesh : ENGINE_MODELS.find(name)->second) {
		Mesh_ptr new_mesh = manager.load_mesh(mesh);
		if (!new_mesh) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "Unable to load mesh in model from resource handler: " << mesh << std::endl;
			errorlogger("Unable to load mesh in model from resource handler: ", mesh.c_str());
			return false;
		}
		meshes.push_back(new_mesh);
	}
	return true;
}