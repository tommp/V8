#include "animation_set.h"

Animation_set::Animation_set(){
	/* Smile and wave boys! */
}

void Animation_set::render_current(const std::string& name, const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, GLfloat rotate){
	if (animations.find(name) != animations.end()) {
		animations[name]->render_current(position, size, color, rotate);
	}
	else{
		errorlogger("ERROR: No animation for current state in Animation_set: ", name.c_str());
		std::cout << "ERROR: No animation for current state in Animation_set: " << name << std::endl;
	}
	
}

bool Animation_set::load_from_file(Resource_manager& resource_manager, const std::string& name){
	if (WORLD_ANIMATION_SETS.find(name) == WORLD_ANIMATION_SETS.end()) {
		std::cout << "ERROR: Animation_set not found!: " << name << std::endl;
		errorlogger("ERROR: Animation_set not found!: ", name.c_str());
		return false;
	}
	
	for (auto it = WORLD_ANIMATION_SETS.find(name)->second.begin(); it != WORLD_ANIMATION_SETS.find(name)->second.end(); ++it) {
		Animation_ptr new_animation = resource_manager.load_animation(*it);
			if (!new_animation) {
				errorlogger("ERROR: Failed to load animation in Animation_set, animation name was: ", (*it).c_str());
				std::cout << "ERROR: Failed to load animation in Animation_set, animation name was: " << (*it).c_str() <<std::endl;
				return false;
			}	
			else{
				animations.insert({(*it), new_animation});
			}
	}
	return true;
}