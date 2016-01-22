#include "./headers/mobs.h"

void Slime_blob::render_frame(){
	animations->render_current(state);
}

void Slime_blob::update_position(float timedelta){
	if ( SDL_GetTicks() > last_move + move_duration && SDL_GetTicks() < last_move + (2*move_duration)) {
		velocity[0] = 0;
		velocity[1] = 0;
	}
	else if(SDL_GetTicks() > last_move + (2*move_duration)){
		velocity[0] = rand()%3 - 1;
		velocity[1] = rand()%3 - 1;
		last_move = SDL_GetTicks();
		move_duration = rand()%1000;
	}

	x += velocity[0] * speed * timedelta;
	y += velocity[1] * speed * timedelta;

}

Slime_blob::Slime_blob(Resource_manager& manager){
	std::string anim = "blob_movement";
	state = {"blob"};
	if ( !(animations = manager.load_animation_set(anim) ) ){
		std::cout << "ERROR: Slime_blob constructor failed to load animation set: " << anim << std::endl;
		errorlogger("ERROR: Slime_blobconstructor failed to load animation set: ", anim.c_str());
	}
	speed = 50;
	x = (rand() + 20) % (10000 - 20);
	y = (rand() + 20) % (10000 - 20);
	z = 0;
	last_move = SDL_GetTicks();
	move_duration = rand()%1000;
	width = TILESIZE;
	height = TILESIZE;
	depth = TILESIZE;
}

void Slime_blob::touch_character(Character& character){
	
}