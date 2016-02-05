#include "resource_manager.h"

Resource_manager::Resource_manager(){

	/* Initialize matrix uniform buffer */
	GLuint uniform_buffer_matrices;
	glGenBuffers(1, &uniform_buffer_matrices);
	  
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_matrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, uniform_buffer_matrices, 0, 2 * sizeof(glm::mat4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if(check_ogl_error()){
		errorlogger("ERROR: Failed to initialize matrix uniform buffer in resource manager!");
		std::cout << "ERROR: Failed to initialize matrix uniform buffer in resource manager! " << std::endl;
		exit(EXIT_FAILURE);
	}

	uniform_buffers["matrices"] = uniform_buffer_matrices;

	/* Initialize button mappings (TODO::read from settings later) */
	button_mappings["player"] = Button_mappings();
}

GLuint Resource_manager::get_uniform_buffer(const std::string& name)const{
	if(uniform_buffers.find(name) != uniform_buffers.end()){
		return uniform_buffers.find(name)->second;
	}
	else{
		errorlogger("ERROR: Uniform buffer not availiable; ", name.c_str());
		std::cout << "ERROR: Uniform buffer not availiable; " << std::endl;
		exit(EXIT_FAILURE);
		return -1;
	}
};

Shader_ptr Resource_manager::load_shader(const std::string& name){
	if (shaders.find(name) != shaders.end()){
		return shaders[name];
	}
	else{
		Shader_ptr new_shader = std::make_shared<Shader>();
		if ( !(new_shader->load_from_file(name)) ){
			std::cout << "ERROR: Resource manager failed to load new shader: " << name << std::endl;
			errorlogger("ERROR: Resource manager failed to load new shader: ", name.c_str());
			return nullptr;
		}
		shaders.insert({name, new_shader});
		return new_shader;
	}
}

Texture_ptr Resource_manager::load_texture(const std::string& name){
	if (textures.find(name) != textures.end()){
		return textures[name];
	}
	else{
		Texture_ptr new_texture = std::make_shared<Texture>();
		if ( !(new_texture->load_from_file(name)) ){
			std::cout << "ERROR: Resource manager failed to load new texture: " << name << std::endl;
			errorlogger("ERROR: Resource manager failed to load new texture: ", name.c_str());
			return nullptr;
		}
		textures.insert({name, new_texture});
		return new_texture;
	}
}

Material_ptr Resource_manager::load_material(const std::string& name){
	if (materials.find(name) != materials.end()){
		return materials[name];
	}
	else{
		Material_ptr new_material = std::make_shared<Material>();
		if ( !(new_material->load_from_file(*this, name)) ){
			std::cout << "ERROR: Resource manager failed to load new material: " << name << std::endl;
			errorlogger("ERROR: Resource manager failed to load new material: ", name.c_str());
			return nullptr;
		}
		materials.insert({name, new_material});
		return new_material;
	}
}

Mesh_ptr Resource_manager::load_mesh(const std::string& name){
	if (meshes.find(name) != meshes.end()){
		return meshes.find(name)->second;
	}
	else{
		Mesh_ptr new_mesh = std::make_shared<Mesh>();
		if ( !(new_mesh->load_from_file(*this, name)) ){
			std::cout << "ERROR: Resource manager failed to load new mesh: " << name << std::endl;
			errorlogger("ERROR: Resource manager failed to load new mesh: ", name.c_str());
			return nullptr;
		}
		meshes.insert({name, new_mesh});
		return new_mesh;
	}
}

Model_ptr Resource_manager::load_model(const std::string& name){
	if (models.find(name) != models.end()){
		return models[name];
	}
	else{
		Model_ptr new_model = std::make_shared<Model>();
		if ( !(new_model->load_from_file(*this, name)) ){
			std::cout << "ERROR: Resource manager failed to load new model: " << name << std::endl;
			errorlogger("ERROR: Resource manager failed to load new model: ", name.c_str());
			return nullptr;
		}
		models.insert({name, new_model});
		return new_model;
	}
}

Animation_ptr Resource_manager::load_animation(const std::string& name){
	if (animations.find(name) != animations.end()){
		return animations[name];
	}
	else{
		Animation_ptr new_animation = std::make_shared<Animation>();
		if ( !(new_animation->load_from_file(*this, name)) ){
			std::cout << "ERROR: Resource manager failed to load new animation: " << name << std::endl;
			errorlogger("ERROR: Resource manager failed to load new animation: ", name.c_str());
			return nullptr;
		}
		animations.insert({name, new_animation});
		return new_animation;
	}
}

Animation_set_ptr Resource_manager::load_animation_set(const std::string& name){
	Animation_set_ptr animation_set = std::make_shared<Animation_set>();

	if (!animation_set->load_from_file(*this, name)) {
		std::cout << "ERROR: Resource manager failed to load new animation: " << name << std::endl;
		errorlogger("ERROR: Resource manager failed to load new animation: ", name.c_str());
		return nullptr;
	}
	return animation_set;
}