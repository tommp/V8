#include "./headers/mobs.h"

void Slime_blob::render_frame(SDL_Renderer &ren){
	animation->render_current(ren, (int)x, (int)y);
}

void Slime_blob::update_position(float timedelta){
	if ( SDL_GetTicks() > last_move + move_duration && SDL_GetTicks() < last_move + (2*move_duration)) {
		vec_x = 0;
		vec_y = 0;
	}
	else if(SDL_GetTicks() > last_move + (2*move_duration)){
		vec_x = rand()%3 - 1;
		vec_y = rand()%3 - 1;
		last_move = SDL_GetTicks();
		move_duration = rand()%1000;
	}

	x += vec_x * speed * timedelta;
	y += vec_y * speed * timedelta;

}

Slime_blob::Slime_blob(SDL_Renderer& ren, Resource_manager& manager){
	std::string anim = "blob";
	if ( !(animation = manager.load_animation(ren, anim) ) ){
		std::cout << "ERROR: Slime_blob constructor failed to load animation: " << anim << std::endl;
		errorlogger("ERROR: Slime_blobconstructor failed to load animation: ", anim.c_str());
	}
	speed = 50;
	x = 200;
	y = 200;
	z = 0;
	vec_y = 0;
	vec_x = 0;
	last_move = SDL_GetTicks();
	move_duration = rand()%1000;
	width = TILESIZE;
	height = TILESIZE;
	depth = TILESIZE;
}