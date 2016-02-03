#include "camera.h"

Camera::Camera(){
	init_position = glm::vec3(0.0f, 300.0f, 200.0f);
	position = glm::vec3(0.0f, 300.0f, 200.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	world_up = glm::vec3(0.0f, 1.0f, 0.0f);
	offset = {-640.0f, 0.0f, 320.0f};

	camera_direction = glm::normalize( position - target);
	right = glm::normalize(glm::cross(world_up, camera_direction));
	camera_up = glm::normalize(glm::cross(camera_direction, right));

	view = glm::lookAt(position, target, camera_up);
}

void Camera::focus_target(const glm::vec3& focus_target){
	position = focus_target + init_position + offset;
	target = focus_target + offset;
}

void Camera::update_view_matrix() {
	/*std::cout << "POS: " << position.x << " : " << position.y << " : " << position.z << std::endl;
	std::cout << "TARG: " << target.x << " : " << target.y << " : " << target.z << std::endl;
	std::cout << "CAM: " << camera_up.x << " : " << camera_up.y << " : " << camera_up.z << std::endl;*/
	view = glm::lookAt(position, target, camera_up);
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