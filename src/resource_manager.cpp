#include "resource_manager.h"

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

Mesh_ptr Resource_manager::load_mesh(const std::string& name){
	if (meshes.find(name) != meshes.end()){
		return meshes[name];
	}
	else{
		Mesh_ptr new_mesh = std::make_shared<Mesh>();
		if ( !(new_mesh->load_from_file(name)) ){
			std::cout << "ERROR: Resource manager failed to load new mesh: " << name << std::endl;
			errorlogger("ERROR: Resource manager failed to load new mesh: ", name.c_str());
			return nullptr;
		}
		meshes.insert({name, new_mesh});
		return new_mesh;
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