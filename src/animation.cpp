#include "./headers/animation.h"

Frame::Frame(int x, int y, int w, int h, int frame_duration){
	duration = frame_duration;
	clip.x = x;
	clip.y = y;
	clip.w = w;
	clip.h = h;
}

Animation::Animation(const Resource_manager& resource_manager, 
					const std::string& texture_name, 
					const std::string& filename) {

	std::ifstream frame_in(filename.c_str());

	if(frame_in.is_open()){
		std::string line;
		int x, y, f_width, f_height, duration;
		while (std::getline(frame_in, line)){			
			std::istringstream iss(line);
			if (!(iss >> duration >> x >> y >> f_width >> f_height)) { 
				errorlogger("Wrong text formating in animation: ", filename.c_str());
				std::cout << "Wrong text formating in animation: " << filename.c_str() << std::endl;
				break; /* BETTER HANDLING HERE */
			} // error
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

		texture = resource_manager.get_texture_ptr(texture_name);

		if(!texture) {
			errorlogger("Unable to load texture from resource handler: ", texture_name.c_str());
			std::cout << "Unable to load texture from resource handler: " << texture_name.c_str() << std::endl;
		}
	}
	current_frame_end = SDL_GetTicks() + current_frame->duration;
}

void Animation::reset_animation(){
	current_frame = frames.begin();
	current_frame_end = SDL_GetTicks() + current_frame->duration;
}

void Animation::render_current(SDL_Renderer *ren, int x, int y){

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