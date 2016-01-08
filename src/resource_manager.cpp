#include "./headers/resource_manager.h"

Texture_ptr Resource_manager::load_texture(SDL_Renderer *ren, const std::string& path, const std::string& name){
	if (textures.find(name) != textures.end()){
		errorlogger("Texture with this name already exists: ", name.c_str());
		std::cout << "Texture with this name already exists: " << name << std::endl;
		return nullptr;
	}
	else{
		Texture_ptr new_texture = std::make_shared<Texture>();
		new_texture->load_from_file(ren, path);
		textures.insert({name, new_texture});
		return new_texture;
	}
}