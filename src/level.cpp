#include "level.h"

Level::Level(int init_width, int init_height, int init_depth, const glm::vec3& pos, const glm::vec3& targ, const glm::vec3& w_up){
	width = init_width;
	height = init_height;
	depth = init_depth;

	camera = Camera(pos, targ, w_up);
}

void Level::center_camera(const Actor* target) {
	/* Center on the actor collision box */
	if(target){
		camera.set_position(*target->get_position());
		camera.set_target(*target->get_position());
		/* Keep the camera in bounds */
		/*
		glm::vec3* cam_pos = camera.get_position();
		if( camera.x < 0 ) { 
			camera.x = 0;
		}
		if( camera.y < 0 ) {
			camera.y = 0;
		}
		if( camera.x > width - camera.w ) {
			camera.x = width - camera.w;
		}
		if( camera.y > height - camera.h ) {
			camera.y = height - camera.h;
		}
		*/
	}
	else{
		errorlogger("ERROR: Centering camera on nullptr in Level class!");
		std::cout << "ERROR: Centering camera on nullptr in Level class!" << std::endl;
	}
}

void Level::load_level(){

}

void Level::unload_level(){

}

void Level::save_level(){

}

void Level::generate_level(){
	
}