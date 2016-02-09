#include "animation_set.h"

Animation_set::Animation_set(){
}

void Animation_set::render_current(const Renderer& renderer, const std::string& name, const glm::vec3& position, const glm::vec3& size, const glm::vec3& direction)const{
	if (animations.find(name) != animations.end()) {
		animations.find(name)->second->animate(renderer, position, size, direction);
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: No animation for current state in Animation_set with state name: " << name << std::endl;
		errorlogger("ERROR: No animation for current state in Animation_set: ", name.c_str());
	}
	
}

bool Animation_set::load_from_file(Resource_manager& resource_manager, const std::string& name){
	if (ENGINE_ANIMATION_SETS.find(name) == ENGINE_ANIMATION_SETS.end()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Animation_set not found in global map! Keyname: " << name << std::endl;
		errorlogger("ERROR: Animation_set not found!: ", name.c_str());
		return false;
	}
	
	for (auto it : ENGINE_ANIMATION_SETS.find(name)->second) {
		Animation_ptr new_animation = resource_manager.load_animation(it);
		if (!new_animation) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Resource manager failed to load animation in Animation_set, animation name was: " << (it).c_str() <<std::endl;
			errorlogger("ERROR: Rescource manager failed to load animation in Animation_set, animation name was: ", (it).c_str());
			return false;
		}	
		else{
			if (animations.find(it) == animations.end()) {
				animations.insert({(it), new_animation});
			}
			else{
				std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Animation already in animation set, animation name: " << (it).c_str() <<std::endl;
				errorlogger("ERROR: Animation already in animation set: ", (it).c_str());
			}
			
		}
	}
	return true;
}