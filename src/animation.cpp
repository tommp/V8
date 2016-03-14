#include "animation.h"

Animation::Animation() {
}

bool Animation::load_from_file(const std::string& name){
	if (!load_binary_animation(name)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Error propogation from load_binary_animation(..) when loading keyname: " << name << std::endl;
		errorlogger("ERROR: Error propogation from load_binary_animation(..) when loading keyname: ", name.c_str());
		return false;
	}
	return true;
}

bool Animation::load_binary_animation(const std::string& name){
	std::string anim_path = ANIMATION_DATA_PATH + name + ".anim";
	std::ifstream contentf(anim_path.c_str(), std::ios::binary);
	GLuint num_channels;

	contentf.read(reinterpret_cast<char *>(&duration), sizeof(GLdouble));
	contentf.read(reinterpret_cast<char *>(&ticks_per_second), sizeof(GLdouble));
	contentf.read(reinterpret_cast<char *>(&num_channels), sizeof(GLuint));
	for (GLuint j = 0; j < num_channels; ++j) {
		GLuint num_pos_keys;
		GLuint num_rot_keys;
		GLuint num_scale_keys;
		GLuint channel_id;

		channels[channel_id].positions = std::vector<std::pair<GLdouble, glm::vec3>>();
		channels[channel_id].scalings = std::vector<std::pair<GLdouble, glm::vec3>>();
		channels[channel_id].rotations = std::vector<std::pair<GLdouble, glm::fquat>>();

		contentf.read(reinterpret_cast<char *>(&channel_id), sizeof(GLuint));

		contentf.read(reinterpret_cast<char *>(&num_pos_keys), sizeof(GLuint));
		channels[channel_id].positions.resize(num_pos_keys);
		for (GLuint k = 0; k < num_pos_keys; ++k) {
			read_vector_from_binary_file(contentf, channels[channel_id].positions[k]);
		}

		contentf.read(reinterpret_cast<char *>(&num_rot_keys), sizeof(GLuint));
		channels[channel_id].rotations.resize(num_rot_keys);
		for (GLuint k = 0; k < num_rot_keys; ++k) {
			read_quaternion_from_binary_file(contentf, channels[channel_id].rotations[k]);
		}

		contentf.read(reinterpret_cast<char *>(&num_scale_keys), sizeof(GLuint));
		channels[channel_id].scalings.resize(num_scale_keys);
		for (GLuint k = 0; k < num_scale_keys; ++k) {
			read_vector_from_binary_file(contentf, channels[channel_id].scalings[k]);
		
		}
	}

	contentf.close();

	return true;
}