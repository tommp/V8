#include "./headers/level.h"

Level::Level(int init_width, int init_height, int init_depth){
	width = init_width;
	height = init_height;
	depth = init_depth;

	camera = {0, 0, level::cam_width, level::cam_height};
}

void Level::center_camera(Actor* target) {
	/* Center on the actor collision box */
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
