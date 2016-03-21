#include "model.h"

Model::Model(){
	name = "";
	state = "";

	model_matrix = glm::mat4();
	model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f, 0.0f));  
	model_matrix = glm::scale(model_matrix, glm::vec3(10.0f, 10.0f, 10.0f));
	normal_model_matrix = glm::inverseTranspose(glm::mat3(model_matrix));

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

		if (!add_lambda_expression(new_mesh)) {
			std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to bind instance lambda expression for mesh: " << mesh << std::endl;
			errorlogger("ERROR: Failed to bind instance lambda expression for mesh: ", mesh.c_str());
			return false;
		}

		meshes.push_back(new_mesh);
	}

	return true;
}

bool Model::add_lambda_expression(const Mesh_ptr& mesh)const{
	std::function<GLboolean(const Shader_ptr& shader)> expression = [&](const Shader_ptr& shader) ->GLboolean {
		glUniformMatrix4fv(shader->load_uniform_location("model"),
						 1, 
						 GL_FALSE, 
						 glm::value_ptr(model_matrix));

		glUniformMatrix3fv(shader->load_uniform_location("normal_model"),
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

	if (!mesh->add_lambda_expression(expression)) {
		std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add lambda expression to mesh context!" << std::endl;
		errorlogger("ERROR: Failed to add lambda expression to mesh context!");
		return false;
	}

	return true;
}

bool Model::update_matrices(const glm::vec3& position, 
							const glm::vec3& scale, 
							const glm::vec3& direction) {
	model_matrix = glm::mat4();
	model_matrix = glm::translate(model_matrix, position);  

	/* TODO:: 3D rotation */
	GLfloat dot = glm::dot(direction, init_direction);
	GLfloat det =  init_direction.x*direction.z - init_direction.z*direction.x;
	GLfloat rotation = -1 * glm::atan(det, dot);

    //model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z)); 
    model_matrix = glm::rotate(model_matrix, rotation, glm::vec3(0.0f, 1.0f, 0.0f)); 
    //model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.5f * size.z));

    model_matrix = glm::scale(model_matrix, glm::vec3(scale));

    normal_model_matrix = glm::inverseTranspose(glm::mat3(model_matrix));

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