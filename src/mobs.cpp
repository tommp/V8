#include "mobs.h"

Cube::Cube(Resource_manager& manager){
	std::string anim_set = "cube";
	state = {"cube_walk"};
	if ( !(animations = manager.load_animation_set(anim_set) ) ){
		std::cout << "ERROR: Slime_blob constructor failed to load animation set: " << anim_set << std::endl;
		errorlogger("ERROR: Slime_blobconstructor failed to load animation set: ", anim_set.c_str());
	}
	speed = 30.0f;
	position[0] = rand() % 2001;
	position[1] = 0;
	position[2] = rand() % 2001;

	velocity = {0.0f, 0.0f, 0.0f};

	last_move = SDL_GetTicks();
	move_duration = rand()%1000;
	size = {6.0f, 6.0f, 6.0f};
}

Cube::~Cube(){
	
}

void Cube::render_frame()const{
	animations->render_current(state, position, size, direction);
}

void Cube::update_position(float timedelta){
	if ( SDL_GetTicks() > last_move + move_duration && SDL_GetTicks() < last_move + (2*move_duration)) {
		velocity[0] = 0;
		velocity[1] = 0;
	}
	else if(SDL_GetTicks() > last_move + (2*move_duration)){
		velocity[0] = rand()%3 - 1;
		velocity[2] = rand()%3 - 1;
		last_move = SDL_GetTicks();
		move_duration = rand()%1000;
		if(glm::length(velocity)){
			velocity = glm::normalize(velocity);
			direction = velocity;
			velocity *= speed;
		}
	}

	last_pos = position;

	position[0] += velocity[0] * timedelta;
	position[2] += velocity[2] * timedelta;

}

void Cube::touch_character(Character& character){
	
}
