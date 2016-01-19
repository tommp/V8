#include "./headers/animation.h"

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