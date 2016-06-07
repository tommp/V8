#include "camera.h"

Camera::Camera(){
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	world_up = glm::vec3(0.0f, 1.0f, 0.0f);
	ortographic = false;

	pitch = 45.0;
	yaw = 90.0;

	target_offset = 20.0;

	SDL_GetMouseState(&(prev_mouse_pos[0]), &(prev_mouse_pos[1]));

	camera_direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	camera_direction.y = sin(glm::radians(pitch));
	camera_direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	right = glm::normalize(glm::cross(world_up, camera_direction));
	camera_up = glm::normalize(glm::cross(camera_direction, right));
}

bool Camera::focus_target(const glm::vec3& focus_target){

	position = focus_target + camera_direction * target_offset;
	target = focus_target;

	return true;
}

bool Camera::center_camera(const glm::vec3& position) {
	/* Center on the actor collision box */
	glm::ivec2 new_mouse_pos;
	SDL_GetMouseState(&(new_mouse_pos[0]), &(new_mouse_pos[1]));

	glm::ivec2 m_diff = prev_mouse_pos - new_mouse_pos;

	prev_mouse_pos = new_mouse_pos;

	yaw += (float)m_diff.x;
	pitch += (float)m_diff.y;

	if(pitch > 89.0f){
		pitch =  89.0f;
	}
	else if(pitch < -89.0f){
		pitch = -89.0f;
	}

	camera_direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	camera_direction.y = sin(glm::radians(pitch));
	camera_direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	right = glm::normalize(glm::cross(world_up, camera_direction));
	camera_up = glm::normalize(glm::cross(camera_direction, right));


	if (focus_target(position)){
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Centering camera on nullptr!" << std::endl;
		errorlogger("ERROR: Centering camera on nullptr!");
		return false;
	}
}