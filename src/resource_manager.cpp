#include "resource_manager.h"

Resource_manager::Resource_manager(){
	/* Initialize button mappings (TODO::read from settings later) */
	button_mappings["player"] = Button_mappings();
}

SDL_Keycode Resource_manager::get_button_map_key(const std::string& map_name, const Key& key)const{
	if (button_mappings.find(map_name) != button_mappings.end()) {
		return button_mappings.find(map_name)->second.get_key(key);
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: No button map with keyname: " << map_name << std::endl;
		errorlogger("ERROR: No button map with keyname: ", map_name.c_str());
		return -1;
	}
}

Shader_ptr Resource_manager::load_shader(const std::string& name){
	if (shaders.find(name) != shaders.end()){
		return shaders[name];
	}
	else{
		Shader_ptr new_shader = std::make_shared<Shader>();
		if ( !(new_shader->load_from_file(name)) ){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Resource manager failed to load new shader with keyname: " << name << std::endl;
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
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Resource manager failed to load new texture with keyname: " << name << std::endl;
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
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Resource manager failed to load new material with keyname: " << name << std::endl;
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
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Resource manager failed to load new mesh with keyname: " << name << std::endl;
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
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Resource manager failed to load new model with keyname: " << name << std::endl;
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
		if (!new_animation->load_from_file(name)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Resource manager failed to load new animation with keyname: " << name << std::endl;
			errorlogger("ERROR: Resource manager failed to load new animation: ", name.c_str());
			return nullptr;
		}
		animations.insert({name, new_animation});
		return new_animation;
	}
}

Animation_set_ptr Resource_manager::load_animation_set(const std::string& name){
	if (animation_sets.find(name) != animation_sets.end()){
		return animation_sets[name];
	}
	else{
		Animation_set_ptr new_animation_set = std::make_shared<Animation_set>();
		if (!new_animation_set->load_from_file(*this, name)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Resource manager failed to load new animation set with keyname: " << name << std::endl;
			errorlogger("ERROR: Resource manager failed to load new animation set: ", name.c_str());
			return nullptr;
		}
		animation_sets.insert({name, new_animation_set});
		return new_animation_set;
	}
}