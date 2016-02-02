#include "animation.h"

Animation::Animation() {
	current_frame_end = 0;
	reverse_looping = true;
	going_forward = true;
}

bool Animation::load_from_file(Resource_manager& resource_manager, const std::string& name){
	
	if (ENGINE_ANIMATIONS.find(name) == ENGINE_ANIMATIONS.end()) {
		std::cout << "ERROR: Animation not found!: " << name << std::endl;
		errorlogger("ERROR: Animation not found!: ", name.c_str());
		return false;
	}

	GLuint datapos = ENGINE_ANIMATIONS.find(name)->second.first;

	std::ifstream contentf (ANIMATION_DATA_FILE_PATH, std::ios::binary);
	if (!contentf.is_open()){
		errorlogger("ERROR: Failed to open content file for animation data!");
		std::cout << "ERROR: Failed to open content file for animation data!" << std::endl;
		return false;
	}

	contentf.seekg(datapos);

	contentf.read(reinterpret_cast<char *>(&reverse_looping), sizeof(bool));

	for (auto it : ENGINE_ANIMATIONS.find(name)->second.second) {
		Model_ptr model = resource_manager.load_model(it);
		if(!model) {
			errorlogger("ERROR: Unable to load model from resource handler: ", name.c_str());
			std::cout << "ERROR: Unable to load model from resource handler: " << name << std::endl;
			return false;
		}
		GLuint frame_duration;
		contentf.read(reinterpret_cast<char *>(&frame_duration), sizeof(GLuint));

		frames.emplace(frames.end(), model, frame_duration);
	}

	contentf.close();

	num_frames = frames.size();
	if (!num_frames) {
		errorlogger("ERROR: No frames loaded in animation: ", name.c_str());
		std::cout << "ERROR: No frames loaded in animation: " << name << std::endl;
		return false;
	}
	current_frame = 0;
	current_frame_end = SDL_GetTicks() + frames[current_frame].get_duration();
	return true;
}

void Animation::reset_animation(){
	current_frame = 0;
	current_frame_end = SDL_GetTicks() + frames[current_frame].get_duration();
}

void Animation::render_frame(const glm::vec3& position, const glm::vec3& size, GLfloat rotate)const{
	frames[current_frame].render(position, size, rotate);
}

void Animation::update_state(){
	if( SDL_GetTicks() > current_frame_end ){
		if(going_forward) {
			current_frame++;
			if( current_frame == num_frames-1) {
				if(!reverse_looping){
					current_frame = 0;
				}
				else{
					going_forward = false;
					current_frame--;
				}
			}
		}
		else{
			current_frame--;
			if( current_frame == 0) {
				going_forward = true;
			}
		}
		current_frame_end = SDL_GetTicks() + frames[current_frame].get_duration();
	}
}