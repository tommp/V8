#include "camera.h"

Camera::Camera(){
	position = glm::vec3(0.0f, 30.0f, 30.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	world_up = glm::vec3(0.0f, 1.0f, 0.0f);

	camera_direction = glm::normalize( position - target);
	right = glm::normalize(glm::cross(world_up, camera_direction));
	camera_up = glm::cross(camera_direction, right);

	view = glm::lookAt(position, position - camera_direction, camera_up);
	offset = {-(SCREEN_WIDTH / 2.0f), 10.0f, (SCREEN_HEIGHT / 2.0f)};

	ortographic = true;
}

void Camera::set_relative_position(const glm::vec3& pos){
	if(!ortographic){
		position =  pos;
	}
	else{
		position = pos + offset;
	}
	
}

void Camera::update_view_matrix() {
	//std::cout << position.x << " : " << position.y << " : " << position.z << std::endl;
	view = glm::lookAt(position, position - camera_direction, world_up);
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