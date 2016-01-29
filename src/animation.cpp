#include "animation.h"

Animation::Animation() {
	current_frame_end = 0;
	reverse_looping = true;
	going_forward = true;
}

bool Animation::load_from_file(Resource_manager& resource_manager, const std::string& name){
	
	if (WORLD_ANIMATIONS.find(name) == WORLD_ANIMATIONS.end()) {
		std::cout << "ERROR: Animation not found!: " << name << std::endl;
		errorlogger("ERROR: Animation not found!: ", name.c_str());
		return false;
	}

	GLuint datapos = WORLD_ANIMATIONS.find(name)->second.first;

	std::ifstream contentf (ANIMATION_DATA_FILE_PATH, std::ios::binary);
	if (!contentf.is_open()){
		errorlogger("ERROR: Failed to open content file for animation data!");
		std::cout << "ERROR: Failed to open content file for animation data!" << std::endl;
		return false;
	}
	contentf.seekg(datapos);

	/* READ ANIMATION DATA HERE */
	contentf.read(reinterpret_cast<char *>(num_frames), sizeof(GLuint));
	contentf.read(reinterpret_cast<char *>(reverse_looping), sizeof(bool));
	contentf.read(reinterpret_cast<char *>(num_frames), sizeof(GLuint));
	

	/* ======================== */
	
	contentf.close();
	/*
	mesh = resource_manager.load_mesh(WORLD_ANIMATIONS.find(name)->second.second);
	if(!mesh) {
		errorlogger("Unable to load mesh from resource handler: ", name.c_str());
		std::cout << "Unable to load mesh from resource handler: " << name.c_str() << std::endl;
		return false;
	}
	*/
	current_frame = 0;
	current_frame_end = SDL_GetTicks() + frames[current_frame].get_duration();
	return true;
}

void Animation::reset_animation(){
	current_frame = 0;
	current_frame_end = SDL_GetTicks() + frames[current_frame].get_duration();
}

void Animation::render_current(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, GLfloat rotate){
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

	/* RENDER HERE */
	frames[current_frame].render(position, size, color, rotate);
	/* =========== */

}