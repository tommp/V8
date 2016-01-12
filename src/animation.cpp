#include "./headers/animation.h"

Frame::Frame(int x, int y, int w, int h, int frame_duration){
	duration = frame_duration;
	clip.x = x;
	clip.y = y;
	clip.w = w;
	clip.h = h;
}

Animation::Animation() {
	texture = NULL;
	current_frame_end = 0;
	reverse_looping = true;
	going_forward = true;
}

bool Animation::load_from_file(SDL_Renderer& ren, Resource_manager& resource_manager, const std::string& name){
	
	if (WORLD_ANIMATIONS.find(name) == WORLD_ANIMATIONS.end()) {
		std::cout << "ERROR: Animation not found!: " << name << std::endl;
		errorlogger("ERROR: Animation not found!: ", name.c_str());
		return false;
	}
	
	std::string filename_txt = WORLD_ANIMATIONS.find(name)->second;
	std::ifstream frame_in(filename_txt.c_str());

	if(frame_in.is_open()){
		std::string line;
		int x, y, f_width, f_height, duration;
		while (std::getline(frame_in, line)){			
			std::istringstream iss(line);
			if (!(iss >> duration >> x >> y >> f_width >> f_height)) { 
				errorlogger("Wrong text formating in animation: ", filename_txt.c_str());
				std::cout << "Wrong text formating in animation: " << filename_txt.c_str() << std::endl;
				return false;
			} /* error */
			else{
				if(frames.empty()){
					frames.emplace_front(x, y, f_width, f_height, duration);
					current_frame = frames.begin();
				}
				frames.emplace_back(x, y, f_width, f_height, duration);
				current_frame++;
			}
		}
		current_frame = frames.begin();

		texture = resource_manager.load_texture(ren, name);

		if(!texture) {
			errorlogger("Unable to load texture from resource handler: ", name.c_str());
			std::cout << "Unable to load texture from resource handler: " << name.c_str() << std::endl;
			return false;
		}
	}
	else{
		errorlogger("ERROR: Unable to open file in Animation::load_from_file: ", filename_txt.c_str());
		std::cout << "ERROR: Unable to open file in Animation::load_from_file: " << filename_txt << std::endl;
		return false;
	}
	current_frame_end = SDL_GetTicks() + current_frame->duration;
	return true;
}

void Animation::reset_animation(){
	current_frame = frames.begin();
	current_frame_end = SDL_GetTicks() + current_frame->duration;
}

void Animation::render_current(SDL_Renderer& ren, int x, int y){
	if( SDL_GetTicks() > current_frame_end ){
		if(going_forward) {
			current_frame++;
			if( current_frame == frames.end()) {
				if(!reverse_looping){
					current_frame = frames.begin();
				}
				else{
					going_forward = false;
					current_frame--;
				}
			}
		}
		else{
			current_frame--;
			if( current_frame == frames.begin()) {
				going_forward = true;
			}
		}
		
		current_frame_end = SDL_GetTicks() + current_frame->duration;
	}

	texture->render(ren, x, y, &(current_frame->clip));
}

Animation_set::Animation_set(){
	/* Smile and be happy! */
}

void Animation_set::render_current(SDL_Renderer& ren, int x, int y, const std::string& name){
	if (animations.find(name) != animations.end()) {
		animations[name]->render_current(ren, x, y);
	}
	else{
		errorlogger("ERROR: No animation for current state in Animation_set: ", name.c_str());
		std::cout << "ERROR: No animation for current state in Animation_set: " << name << std::endl;
	}
	
}

bool Animation_set::load_from_file(SDL_Renderer& ren, Resource_manager& resource_manager, const std::string& name){
	if (WORLD_ANIMATION_SETS.find(name) == WORLD_ANIMATION_SETS.end()) {
		std::cout << "ERROR: Animation_set not found!: " << name << std::endl;
		errorlogger("ERROR: Animation_set not found!: ", name.c_str());
		return false;
	}
	
	std::string filename_txt = WORLD_ANIMATION_SETS.find(name)->second;
	std::ifstream animation_names(filename_txt.c_str());

	if(animation_names.is_open()){
		std::string animation_name;
		while (std::getline(animation_names, animation_name)){	
			Animation_ptr new_animation = resource_manager.load_animation(ren, animation_name);
			if (! new_animation) {
				errorlogger("ERROR: Failed to load animation in Animation_set: ", animation_name.c_str());
			}	
			else{
				animations.insert({animation_name, new_animation});
			}
		}
	}
	else{
		errorlogger("ERROR: Unable to open file in Animation_set::load_from_file: ", filename_txt.c_str());
		std::cout << "ERROR: Unable to open file in Animation_set::load_from_file: " << filename_txt << std::endl;
		return false;
	}
	return true;
}