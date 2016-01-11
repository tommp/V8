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
				frames.emplace_after(current_frame, x, y, f_width, f_height, duration);
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
	current_frame_end = SDL_GetTicks() + current_frame->duration;
	return true;
}

void Animation::reset_animation(){
	current_frame = frames.begin();
	current_frame_end = SDL_GetTicks() + current_frame->duration;
}

void Animation::render_current(SDL_Renderer& ren, int x, int y){
	if( SDL_GetTicks() > current_frame_end ){
		current_frame++;
		if( current_frame != frames.end()) {
		}
		else{
			current_frame = frames.begin();
		}
		current_frame_end = SDL_GetTicks() + current_frame->duration;
	}

	texture->render(ren, x, y, &(current_frame->clip));
}