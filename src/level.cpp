#include "level.h"

Level::Level(int init_width, int init_height, int init_depth){
	width = init_width;
	height = init_height;
	depth = init_depth;

	camera = Camera();
}

void Level::center_camera(const Actor* target) {
	/* Center on the actor collision box */
	if(target){
		camera.set_relative_position(*target->get_position());
		camera.set_target(*target->get_position());

		/* Keep the camera in bounds */
		if( camera.get_x() < 0.0f ) { 
			camera.set_x(0.0f);
		}
		if( camera.get_z() - SCREEN_HEIGHT  < 0.0f ) {
			camera.set_z(SCREEN_HEIGHT);
		}
		if( camera.get_x() + SCREEN_WIDTH  > width) {
			camera.set_x(width - SCREEN_WIDTH);
		}
		if( camera.get_z() > height) {
			camera.set_z(height);
		}
		
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