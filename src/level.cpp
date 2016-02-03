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
		camera.focus_target(*target->get_position());

		/* Keep the camera in bounds */
		if( target->get_position()->x < (SCREEN_WIDTH/2.0f) ) { 
			camera.set_x(SCREEN_WIDTH/2.0f);
			camera.set_tx(SCREEN_WIDTH/2.0f);
		}
		if( target->get_position()->z < (SCREEN_HEIGHT/2.0f) ) {
			camera.set_z(SCREEN_HEIGHT/2.0f);
			camera.set_tz(SCREEN_HEIGHT/2.0f);
		}
		if( (target->get_position()->x + (SCREEN_WIDTH/2.0f)) > width) {
			camera.set_x(width - (SCREEN_WIDTH/2.0f));
			camera.set_tx(width - (SCREEN_WIDTH/2.0f));
		}
		if( (target->get_position()->z + (SCREEN_HEIGHT/2.0f)) > height) {
			camera.set_z(height - (SCREEN_HEIGHT/2.0f));
			camera.set_tz(height - (SCREEN_HEIGHT/2.0f));
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