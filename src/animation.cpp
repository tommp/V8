#include "animation.h"

Animation::Animation() {
	reverse_looping = true;
	going_forward = true;
}

void Animation::animate(const Renderer& renderer, const glm::vec3& position, const glm::vec3& size, const glm::vec3& direction){
	update_state();
}

void Animation::update_state(){
}