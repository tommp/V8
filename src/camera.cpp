#include "camera.h"

Camera::Camera(){
	position = glm::vec3(0.0f, 0.0f, 3.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	world_up = glm::vec3(0.0f, 1.0f, 0.0f);

	direction = glm::normalize(position - target);
	right = glm::normalize(glm::cross(world_up, direction));
	up = glm::cross(direction, right);
	view = glm::lookAt(position, target, world_up);
}

Camera::Camera(const glm::vec3& pos, const glm::vec3& targ, const glm::vec3& w_up){
	position = pos;
	target = targ;
	world_up = w_up;

	direction = glm::normalize(position - target);
	right = glm::normalize(glm::cross(world_up, direction));
	up = glm::cross(direction, right);
	view = glm::lookAt(position, target, world_up);
}

void Camera::update_view_matrix() {
	view = glm::lookAt(position, target, world_up);
}