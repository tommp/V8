#include "headers/level.h"

Level::Level(int init_width, int init_height, int init_depth){
	width = init_width;
	height = init_height;
	depth = init_depth;

	camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
}

void Level::center_camera(const Actor* target) {
	/* Center on the actor collision box */
	if(target){
		camera.x = ( target->get_x() + target->get_width() / 2 ) - camera.w / 2;
		camera.y = ( target->get_y() + target->get_height() / 2 ) - camera.h / 2;

		/* Keep the camera in bounds */
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