#include "./headers/resource_manager.h"

Texture_ptr Resource_manager::load_texture(SDL_Renderer& ren, const std::string& name){
	if (textures.find(name) != textures.end()){
		return textures[name];
	}
	else{
		Texture_ptr new_texture = std::make_shared<Texture>();
		if ( !( new_texture->load_from_file(ren, name) ) ){
			std::cout << "ERROR: Resource manager failed to load new texture: " << name << std::endl;
			errorlogger("ERROR: Resource manager failed to load new texture: ", name.c_str());
			return nullptr;
		}
		textures.insert({name, new_texture});
		return new_texture;
	}
}

Animation_ptr Resource_manager::load_animation(SDL_Renderer& ren, const std::string& name){
	if (animations.find(name) != animations.end()){
		return animations[name];
	}
	else{
		Animation_ptr new_animation = std::make_shared<Animation>();
		if ( !(new_animation->load_from_file(ren, *this, name) ) ){
			std::cout << "ERROR: Resource manager failed to load new animation: " << name << std::endl;
			errorlogger("ERROR: Resource manager failed to load new animation: ", name.c_str());
			return nullptr;
		}
		animations.insert({name, new_animation});
		return new_animation;
	}
}

Animation_set_ptr Resource_manager::load_animation_set(SDL_Renderer& ren, const std::string& name){
	Animation_set_ptr animation_set = std::make_shared<Animation_set>();

	if (!animation_set->load_from_file(ren, *this, name)) {
		std::cout << "ERROR: Resource manager failed to load new animation: " << name << std::endl;
		errorlogger("ERROR: Resource manager failed to load new animation: ", name.c_str());
		return nullptr;
	}
	return animation_set;
}