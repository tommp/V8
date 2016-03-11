#include "vertex.h"

void initialize_vertex(Vertex& vertex){
	vertex.position = {0.0f, 0.0f, 0.0f};
	vertex.normal = {0.0f, 0.0f, 0.0f};
	vertex.tex_coords = {0.0f, 0.0f};
	vertex.bone_ids = {-1, -1, -1, -1};
	vertex.bone_weights = {0.0f, 0.0f, 0.0f, 0.0f};
}

bool add_bone_to_vertex(Vertex& vertex, GLuint index){
	for (GLuint i = 0; i < 4; ++i) {
		if (vertex.bone_ids[i] == -1) {
			vertex.bone_ids[i] = index;
			return true;
		}
	}
	std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Bone limit reached for vertex!"<< std::endl;
	errorlogger("ERROR: Bone limit reached for vertex!");
	return false;
}
bool add_weight_to_vertex(Vertex& vertex, GLfloat weight){
	for (GLuint i = 0; i < 4; ++i) {
		if (vertex.bone_weights[i] == 0.0f) {
			vertex.bone_weights[i] = weight;
			return true;
		}
	}
	std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Weight limit reached for vertex!"<< std::endl;
	errorlogger("ERROR: Weight limit reached for vertex!");
	return false;
}