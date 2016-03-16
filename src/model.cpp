#include "model.h"

Model::Model(){
}

bool Model::add_bases_to_context(Rendering_context& context)const{
	GLboolean add_successful = true;
	for (auto mesh : meshes) {
		if (!mesh->add_base_to_context(context)) {
			add_successful = false;
		}
	}

	if (!add_successful) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to update mesh context for model: " << name << std::endl;
		errorlogger("ERROR: Failed to update mesh context for model: ", name.c_str());
		return false;
	}

	return true;
}

bool Model::load_binary_model(Resource_manager& manager, const std::string& name, std::vector<std::string>& meshes){
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

	contentf.read(reinterpret_cast<char *>(&is_animated), sizeof(GLuint));
	if (is_animated) {
		animations = manager.load_animation_set(name);
		if (!animations) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Unable to load animation set in model from resource handler: " << name << std::endl;
			errorlogger("ERROR: Unable to load animation set in model from resource handler: ", name.c_str());
			return false;
		}
	}

	contentf.close();

	return true;
}

bool Model::load_from_file(Resource_manager& manager, const std::string& name){
	this->name = name;
	std::vector<std::string> mesh_names;

	if (!load_binary_model(manager, name, mesh_names)) {
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