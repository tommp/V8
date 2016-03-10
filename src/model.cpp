#include "model.h"

Model::Model(){
}

void Model::render_model(const Renderer& renderer, const glm::vec3& position, const glm::vec3& size, const glm::vec3& direction)const{
	for (auto mesh : meshes) {
		mesh->render_mesh(renderer, position, size, direction);
	}
}

bool Model::load_binary_model(const std::string& name, std::vector<std::string>& meshes){
	std::string model_path = MODEL_DATA_PATH + name + ".model";

	std::ifstream contentf (model_path.c_str(), std::ios::binary);
	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for model data: " << model_path << std::endl;
		errorlogger("ERROR: Failed to open content file for model data: ", model_path.c_str());
		return false;
	}

	GLuint num_meshes = 0;

	contentf.read(reinterpret_cast<char *>(&num_meshes), sizeof(GLuint));

	for (GLuint i = 0; i < num_meshes; ++i) {
		std::string mesh;
		if (!read_string_from_binary_file(contentf, mesh)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Unable to read mesh name in model from file: " << model_path << std::endl;
			errorlogger("ERROR: Unable to read mesh name in model from file: ", model_path.c_str());
			return false;
		}
		meshes.push_back(mesh);
	}

	contentf.close();

	return true;
}

bool Model::load_from_file(Resource_manager& manager, const std::string& name){

	std::vector<std::string> mesh_names;

	if (!load_binary_model(name, mesh_names)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Unable to load binary model with name: " << name << std::endl;
		errorlogger("ERROR: Unable to load binary model with name: ", name.c_str());
		return false;
	}

	for (auto mesh : mesh_names) {
		Mesh_ptr new_mesh = manager.load_mesh(mesh);
		if (!new_mesh) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Unable to load mesh in model from resource handler: " << mesh << std::endl;
			errorlogger("ERROR: Unable to load mesh in model from resource handler: ", mesh.c_str());
			return false;
		}
		meshes.push_back(new_mesh);
	}
	return true;
}