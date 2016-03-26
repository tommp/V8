#include "model.h"

GLuint Model::context_count= 0;

Model::Model(){
	name = "";
	is_complete = false;

	init_direction = {0.0f, 0.0f, -1.0f};

	is_animated = false;
	animations = nullptr;
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

	if (name == Mesh_vars::BOX) {
		mesh_names.push_back(name);
	}
	else{
		if (!load_binary_model(manager, name, mesh_names)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Unable to load binary model with name: " << name << std::endl;
			errorlogger("ERROR: Unable to load binary model with name: ", name.c_str());
			return false;
		}
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

bool Model::bind_context(const glm::mat4& model_matrix,
								const glm::mat3& normal_model_matrix, 
								std::string& context_name){
	std::function<GLboolean(const Shader_ptr& shader, GLuint instance)> expression = [&](const Shader_ptr& shader, GLuint instance) ->GLboolean {
		glUniformMatrix4fv(shader->load_uniform_location("models", instance),
						 1, 
						 GL_FALSE, 
						 glm::value_ptr(model_matrix));

		glUniformMatrix3fv(shader->load_uniform_location("normal_models", instance),
						 1, 
						 GL_FALSE, 
						 glm::value_ptr(normal_model_matrix));

		if(check_ogl_error()) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to bind uniform matrices for model!" << std::endl;
			errorlogger("ERROR: Failed to bind uniform matrices for model");
			return false;
		}

		return true;
	};

	context_name = static_cast<std::ostringstream*>( &(std::ostringstream() << name << "_" << context_count++) )->str();

	for (auto mesh : meshes) {
		if (!mesh->add_uniform_setup(context_name, expression)) {
			std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bind uniforms to mesh context!" << std::endl;
			errorlogger("ERROR: Failed to bind uniforms to mesh context!");
			return false;
		}
	}

	is_complete = true;

	return true;
}

bool Model::unbind_context(std::string& context_name){
	for (auto mesh : meshes) {
		if (!mesh->remove_uniform_setup(context_name)) {
			std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to remove model mesh context: " << context_name << std::endl;
			errorlogger("ERROR: Failed to remove model mesh context: ", context_name.c_str());
			return false;
		}
	}

	return true;
}

bool Model::add_mesh_contexts_to_renderer(Renderer& renderer)const{
	for (auto mesh : meshes) {
		if (!mesh->add_context_to_renderer(renderer)) {
			std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add mesh context to renderer!" << std::endl;
			errorlogger("ERROR: Failed to add mesh context to renderer!");
			return false;
		}
	}
	return true;
}

bool Model::add_light_contexts_to_renderer(Renderer& renderer)const{
	return true;
}

bool Model::add_contexts_to_renderer(Renderer& renderer)const{
	if (!is_complete) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add rendering contexts, matrices not set for model: " << name << std::endl;
		errorlogger("ERROR: Failed to add rendering contexts, matrices not set for model: ", name.c_str());
		return false;
	}

	if (!add_mesh_contexts_to_renderer(renderer)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add mesh rendering contexts to renderer for model: " << name << std::endl;
		errorlogger("ERROR: Failed to add mesh rendering contexts to renderer for model: ", name.c_str());
		return false;
	}

	if (!add_light_contexts_to_renderer(renderer)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add light rendering contexts to renderer for model: " << name << std::endl;
		errorlogger("ERROR: Failed to add light rendering contexts to renderer for model: ", name.c_str());
		return false;
	}

	return true;
}

glm::vec3& Model::get_init_direction(){
	return init_direction;
}