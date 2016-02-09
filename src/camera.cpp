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

void Camera::upload_view_matrix(GLuint matrix_uniform_buffer)const{
	glBindBuffer(GL_UNIFORM_BUFFER, matrix_uniform_buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);  

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to upload view matrix: " << matrix_uniform_buffer << std::endl;
		errorlogger("ERROR: Failed to upload view matrix!");
		exit(EXIT_FAILURE);
	}
}

void Camera::center_camera(const Actor_ptr& target, GLuint bound_width, GLuint bound_height) {
	/* Center on the actor collision box */
	if(target){
		focus_target(*target->get_position());

		/* Keep the camera in bounds */
		if( target->get_position()->x < (SCREEN_WIDTH/2.0f) ) { 
			set_x(SCREEN_WIDTH/2.0f);
			set_tx(SCREEN_WIDTH/2.0f);
		}
		if( target->get_position()->z < (SCREEN_HEIGHT/2.0f) ) {
			set_z(SCREEN_HEIGHT/2.0f);
			set_tz(SCREEN_HEIGHT/2.0f);
		}
		if( (target->get_position()->x + (SCREEN_WIDTH/2.0f)) > bound_width) {
			set_x(bound_width - (SCREEN_WIDTH/2.0f));
			set_tx(bound_width - (SCREEN_WIDTH/2.0f));
		}
		if( (target->get_position()->z + (SCREEN_HEIGHT/2.0f)) > bound_height) {
			set_z(bound_height - (SCREEN_HEIGHT/2.0f));
			set_tz(bound_height - (SCREEN_HEIGHT/2.0f));
		}
		
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Centering camera on nullptr!" << std::endl;
		errorlogger("ERROR: Centering camera on nullptr!");
	}
}