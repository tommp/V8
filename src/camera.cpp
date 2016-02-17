#include "camera.h"

Camera::Camera(){
	init_position = glm::vec3(0.0f, 300.0f, 400.0f);
	position = glm::vec3(0.0f, 300.0f, 400.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	world_up = glm::vec3(0.0f, 1.0f, 0.0f);
	offset = {0.0f,0.0f,0.0f};//{-640.0f, 0.0f, 320.0f};

	camera_direction = glm::normalize( position - target);
	right = glm::normalize(glm::cross(world_up, camera_direction));
	camera_up = glm::normalize(glm::cross(camera_direction, right));
}

void Camera::focus_target(const glm::vec3& focus_target){
	position = focus_target + init_position + offset;
	target = focus_target + offset;
}

void Camera::center_camera(const Actor_ptr& target) {
	/* Center on the actor collision box */
	if(target){
		focus_target(*target->get_position());

		/*
		if( target->get_position()->x < (screen_width/2.0f) ) { 
			set_x(screen_width/2.0f);
			set_tx(screen_width/2.0f);
		}
		if( target->get_position()->z < (screen_height/2.0f) ) {
			set_z(screen_height/2.0f);
			set_tz(screen_height/2.0f);
		}
		if( (target->get_position()->x + (screen_width/2.0f)) > bound_width) {
			set_x(bound_width - (screen_width/2.0f));
			set_tx(bound_width - (screen_width/2.0f));
		}
		if( (target->get_position()->z + (screen_height/2.0f)) > bound_height) {
			set_z(bound_height - (screen_height/2.0f));
			set_tz(bound_height - (screen_height/2.0f));
		}*/
		
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Centering camera on nullptr!" << std::endl;
		errorlogger("ERROR: Centering camera on nullptr!");
	}
}