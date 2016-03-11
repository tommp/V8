#include "animation_set.h"

Animation_set::Animation_set(){
}

bool Animation_set::load_from_file(Resource_manager& resource_manager, const std::string& name){
	if (!load_binary_animation_set(resource_manager, name)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Error propogation from load_binary_animation_set(..) when loading keyname: " << name << std::endl;
		errorlogger("ERROR: Error propogation from load_binary_animation_set(..) when loading keyname: ", name.c_str());
		return false;
	}

	return true;
}

bool Animation_set::load_binary_animation_set(Resource_manager& resource_manager, const std::string& name){
	std::string anim_set_path = ANIMATION_DATA_PATH + name + ".anims";
	std::ifstream contentf(anim_set_path.c_str(), std::ios::binary);
	std::vector<std::string> animation_set_nodes;

	GLuint num_animations;
	contentf.read(reinterpret_cast<char *>(&num_animations), sizeof(GLuint));
	for (GLuint i = 0; i < num_animations; ++i) {
		std::string anim_name;
		if (!read_string_from_binary_file(contentf, anim_name)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Could not load animation in set: " << anim_set_path << std::endl;
			errorlogger("FATAL ERROR: Could not load animation in set: ", anim_set_path.c_str());
			exit(EXIT_FAILURE);
		}
		Animation_ptr new_animation = resource_manager.load_animation(anim_name);
		if (!new_animation) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to load animation in set with animation name: " << anim_name << std::endl;
			errorlogger("ERROR: Failed to load animation in set with animation name: ", anim_name.c_str());
			return false;
		}

		animations.insert({anim_name, new_animation});
	}

	if (!load_binary_skeleton(contentf)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to load binary skeleyon in animation set name: " << name << std::endl;
		errorlogger("ERROR: Failed to load binary skeleyon in animation set name: ", name.c_str());
		return false;
	}

	GLuint reading_tree;
	contentf.read(reinterpret_cast<char *>(&reading_tree), sizeof(GLuint));
	if (reading_tree != 0 || reading_tree != 1) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Corrupted file data read from: " << name << std::endl;
		errorlogger("FATAL ERROR: Corrupted file data read from: ", name.c_str());
		exit(EXIT_FAILURE);
	}

	std::unordered_map<std::string, Skeletal_node_ptr> bone_map;

	while (reading_tree) {
		std::string node_id;
		std::string parent_id;
		glm::mat4 transformation;
		Skeletal_node_ptr new_node = std::make_shared<Skeletal_node>();

		read_string_from_binary_file(contentf, node_id);
		read_string_from_binary_file(contentf, parent_id);
		read_string_from_binary_file(contentf, new_node->name);
		for (GLuint x = 0; x < 4; ++x) {
			for (GLuint y = 0; y < 4; ++y) {
				contentf.read(reinterpret_cast<char *>(&(transformation[x][y])), sizeof(GLfloat));
			}
		}

		new_node->trans = transformation;

		/* Safe as long as we always write a nodes parent before its children, whitch we do */
		if (parent_id != "0") {
			new_node->parent = bone_map[parent_id];
			bone_map[parent_id]->children.push_back(new_node);
		}
		else{
			new_node->parent = nullptr;
			root_node = new_node;
		}

		bone_map[node_id] = new_node;

		contentf.read(reinterpret_cast<char *>(&reading_tree), sizeof(GLuint));
	}

	return true;
}

bool Animation_set::load_binary_skeleton(std::ifstream& contentf){
	return true;
}