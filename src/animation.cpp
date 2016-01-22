#include "./headers/animation.h"

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
	GLuint datapos = WORLD_ANIMATIONS.find(name)->second;

	std::ifstream contentf (ANIMATION_DATA_FILE_PATH, std::ios::binary);
	if (!contentf.is_open()){
		errorlogger("ERROR: Failed to open content file for animation data!");
		std::cout << "ERROR: Failed to open content file for animation data!" << std::endl;
		return false;
	}
	contentf.seekg(datapos);

	contentf.read(reinterpret_cast<char *>(&set_size), sizeof(GLuint));
	glGenVertexArrays(set_size, VAO_set);
    glGenBuffers(set_size, VBO_set);

	for (unsigned int i = 0; i < set_size; i++){
		GLfloat* vertices = nullptr;
		GLuint vertex_set_size = 0;
		GLuint frame_duration = 0;
		contentf.read(reinterpret_cast<char *>(&vertex_set_size), sizeof(GLuint));
		contentf.read(reinterpret_cast<char *>(&frame_duration), sizeof(GLuint));
		contentf.read(reinterpret_cast<char *>(vertices), sizeof(GLfloat)*vertex_set_size);

		frame_durations.push_back(frame_duration);

	    glBindVertexArray(VAO_set[i]);

	    glBindBuffer(GL_ARRAY_BUFFER, VBO_set[i]);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex_set_size, vertices, GL_DYNAMIC_DRAW);

	    /* Position attribute */
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
	    glEnableVertexAttribArray(0);

	    /* Colour */
	    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	    glEnableVertexAttribArray(0);
	    
	    /* TexCoord attribute */
	    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
	    glEnableVertexAttribArray(2);

	    glBindBuffer(GL_ARRAY_BUFFER, 0);
	    glBindVertexArray(0); // Unbind VAO

	    free(vertices);
	}
	
	contentf.close();

	texture = resource_manager.load_texture(name);
	if(!texture) {
		errorlogger("Unable to load texture from resource handler: ", name.c_str());
		std::cout << "Unable to load texture from resource handler: " << name.c_str() << std::endl;
		return false;
	}
	current_frame = 0;
	current_frame_end = SDL_GetTicks() + frame_durations[current_frame];
	return true;
}

void Animation::reset_animation(){
	current_frame = 0;
	current_frame_end = SDL_GetTicks() + frame_durations[current_frame];
}

void Animation::render_current(){
	if( SDL_GetTicks() > current_frame_end ){
		if(going_forward) {
			current_frame++;
			if( current_frame == set_size-1) {
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
		
		current_frame_end = SDL_GetTicks() + frame_durations[current_frame];
	}

	/* RENDER HERE */

}