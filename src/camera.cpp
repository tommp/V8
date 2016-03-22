#include "camera.h"

Camera::Camera(){
	init_position = glm::vec3(0.0f, 300.0f, 400.0f);
	position = glm::vec3(0.0f, 300.0f, 400.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	world_up = glm::vec3(0.0f, 1.0f, 0.0f);
	offset = {0.0f,0.0f,0.0f};//{-640.0f, 0.0f, 320.0f};
	ortographic = false;

	camera_direction = glm::normalize( position - target);
	right = glm::normalize(glm::cross(world_up, camera_direction));
	camera_up = glm::normalize(glm::cross(camera_direction, right));
}

bool Camera::focus_target(const glm::vec3& focus_target){
	position = focus_target + init_position + offset;
	target = focus_target + offset;

	return true;
}

bool Camera::center_camera(const glm::vec3& position) {
	/* Center on the actor collision box */
	if (focus_target(position)){

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
		return true;
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Centering camera on nullptr!" << std::endl;
		errorlogger("ERROR: Centering camera on nullptr!");
		return false;
	}
}