#include "./headers/animation_set.h"

Animation_set::Animation_set(){
	/* Smile and wave boys! */
}

void Animation_set::render_current(SDL_Renderer& ren, int x, int y, const std::string& name){
	if (animations.find(name) != animations.end()) {
		animations[name]->render_current(ren, x, y);
	}
	else{
		errorlogger("ERROR: No animation for current state in Animation_set: ", name.c_str());
		std::cout << "ERROR: No animation for current state in Animation_set: " << name << std::endl;
	}
	
}

bool Animation_set::load_from_file(SDL_Renderer& ren, Resource_manager& resource_manager, const std::string& name){
	if (WORLD_ANIMATION_SETS.find(name) == WORLD_ANIMATION_SETS.end()) {
		std::cout << "ERROR: Animation_set not found!: " << name << std::endl;
		errorlogger("ERROR: Animation_set not found!: ", name.c_str());
		return false;
	}
	
	std::string filename_txt = WORLD_ANIMATION_SETS.find(name)->second;
	std::ifstream animation_names(filename_txt.c_str());

	if(animation_names.is_open()){
		std::string animation_name;
		while (std::getline(animation_names, animation_name)){	
			Animation_ptr new_animation = resource_manager.load_animation(ren, animation_name);
			if (! new_animation) {
				errorlogger("ERROR: Failed to load animation in Animation_set, animation name was: ", animation_name.c_str());
			}	
			else{
				animations.insert({animation_name, new_animation});
			}
		}
	}
	else{
		errorlogger("ERROR: Unable to open file in Animation_set::load_from_file: ", filename_txt.c_str());
		std::cout << "ERROR: Unable to open file in Animation_set::load_from_file: " << filename_txt << std::endl;
		return false;
	}
	return true;
}