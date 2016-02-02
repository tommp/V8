#include "camera.h"

Camera::Camera(){
	position = glm::vec3(30.0f, 30.0f, 30.0f);
	init_position = glm::vec3(30.0f, 30.0f, 30.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	world_up = glm::vec3(0.0f, 1.0f, 0.0f);

	camera_direction = glm::normalize( position - target);
	right = glm::normalize(glm::cross(world_up, camera_direction));
	camera_up = glm::cross(camera_direction, right);

	view = glm::lookAt(position, position - camera_direction, camera_up);
}

Camera::Camera(const glm::vec3& pos, const glm::vec3& targ, const glm::vec3& w_up){
	position = pos;
	init_position = pos;
	target = targ;
	world_up = w_up;

	camera_direction = glm::normalize(position + target);
	right = glm::normalize(glm::cross(world_up, camera_direction));
	camera_up = glm::cross(camera_direction, right);
	view = glm::lookAt(position, position - camera_direction, camera_up);
}

void Camera::update_view_matrix() {
	std::cout << position.x << " : " << position.y << " : " << position.z << std::endl;
	//camera_direction = glm::normalize(position + target);
	//right = glm::normalize(glm::cross(world_up, camera_direction));
	//camera_up = glm::cross(camera_direction, right);
	view = glm::lookAt(position, position - camera_direction, camera_up);
}

void Camera::upload_view_matrix(GLuint matrix_uniform_buffer){
	glBindBuffer(GL_UNIFORM_BUFFER, matrix_uniform_buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);  

	if(check_ogl_error()){
		errorlogger("ERROR: Failed to upload view matrix!");
		std::cout << "ERROR: Failed to upload view matrix: " << matrix_uniform_buffer << std::endl;
		exit(EXIT_FAILURE);
	}
}