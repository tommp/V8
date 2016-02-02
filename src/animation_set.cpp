#include "animation_set.h"

Animation_set::Animation_set(){
}

void Animation_set::render_current(const std::string& name, const glm::vec3& position, const glm::vec3& size, GLfloat rotate)const{
	if (animations.find(name) != animations.end()) {
		animations.find(name)->second->render_frame(position, size, rotate);
	}
	else{
		errorlogger("ERROR: No animation for current state in Animation_set: ", name.c_str());
		std::cout << "ERROR: No animation for current state in Animation_set: " << name << std::endl;
	}
	
}

bool Animation_set::load_from_file(Resource_manager& resource_manager, const std::string& name){
	if (ENGINE_ANIMATION_SETS.find(name) == ENGINE_ANIMATION_SETS.end()) {
		std::cout << "ERROR: Animation_set not found!: " << name << std::endl;
		errorlogger("ERROR: Animation_set not found!: ", name.c_str());
		return false;
	}
	
	for (auto it : ENGINE_ANIMATION_SETS.find(name)->second) {
		Animation_ptr new_animation = resource_manager.load_animation(it);
		if (!new_animation) {
			errorlogger("ERROR: Failed to load animation in Animation_set, animation name was: ", (it).c_str());
			std::cout << "ERROR: Failed to load animation in Animation_set, animation name was: " << (it).c_str() <<std::endl;
			return false;
		}	
		else{
			if (animations.find(it) == animations.end()) {
				animations.insert({(it), new_animation});
			}
			else{
				errorlogger("ERROR: Animation already in animation set: ", (it).c_str());
				std::cout << "ERROR: Animation already in animation set: " << (it).c_str() <<std::endl;
			}
			
		}
	}
	return true;
}