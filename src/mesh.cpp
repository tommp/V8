#include "mesh.h"

Mesh::Mesh(){
	VBO = 0;
	EBO = 0;
	base_context = std::make_shared<Base_render_context>();
	base_context->VAO = 0;
	base_context->render_mode = GL_FILL;
	base_context->object_color = {1.0f, 1.0f, 1.0f, 1.0f};
}

Mesh::~Mesh(){
	free_mesh();
}

bool Mesh::load_binary_mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::string& material_name){
	std::string mesh_path = MESH_DATA_PATH + name + ".mesh";

	this->name = name;

	std::ifstream contentf (mesh_path.c_str(), std::ios::binary);
	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for mesh data: " << mesh_path << std::endl;
		errorlogger("ERROR: Failed to open content file for mesh data: ", mesh_path.c_str());
		return false;
	}

	Vertex vertex;
	GLuint index;
	GLuint vsize;
	GLuint isize;
	char has_material;
	char has_animation;

	contentf.read(reinterpret_cast<char *>(&has_material), sizeof(char));
	if (has_material){
		read_string_from_binary_file(contentf, material_name);
	}

	contentf.read(reinterpret_cast<char *>(&vsize), sizeof(GLuint));
	contentf.read(reinterpret_cast<char *>(&isize), sizeof(GLuint));
	for (GLuint i = 0; i < vsize; ++i) {
		contentf.read(reinterpret_cast<char *>(&vertex), sizeof(Vertex));
		vertices.push_back(vertex);
	}
	for (GLuint i = 0; i < isize; ++i) {
		contentf.read(reinterpret_cast<char *>(&index), sizeof(GLuint));
		indices.push_back(index);
	}

	contentf.read(reinterpret_cast<char *>(&has_animation), sizeof(char));
	if (has_animation) {
		for (GLuint i = 0; i < 4; ++i) {
			for (GLuint j = 0; j < 4; ++j) {
				contentf.read(reinterpret_cast<char *>(&(root_inverse_transform[i][j])), sizeof(float));
			}
		}
		GLuint num_bones;
		contentf.read(reinterpret_cast<char *>(&num_bones), sizeof(GLuint));
		for (GLuint i = 0; i < num_bones; ++i) {
			glm::mat4 bone_offset;
			for (GLuint x = 0; x < 4; ++x) {
				for (GLuint y = 0; y < 4; ++y) {
					contentf.read(reinterpret_cast<char *>(&(bone_offset[x][y])), sizeof(float));
				}
			}
			bone_info.push_back(bone_offset);
		}

		GLuint num_bone_mappings;
		contentf.read(reinterpret_cast<char *>(&num_bone_mappings), sizeof(GLuint));
		for (GLuint i = 0; i < num_bone_mappings; ++i) {
			GLuint bone_id;
			GLuint bone_index;
			contentf.read(reinterpret_cast<char *>(&(bone_id)), sizeof(GLuint));
			contentf.read(reinterpret_cast<char *>(&(bone_index)), sizeof(GLuint));
			bone_map[bone_id] = bone_id;
		}

		base_context->shader_type = GEOMETRY_ANIMATED;
	}
	else{
		base_context->shader_type = GEOMETRY_STATIC;
	}

	contentf.close();

	return true;
}

bool Mesh::load_from_file(Resource_manager& manager, const std::string& name){

	/* Get rid of preexisting mesh */
	if( VBO != 0 ){
		free_mesh();
	}

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::string material_name;

	if (!load_binary_mesh(name, vertices, indices, material_name)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Error propogation from load_binary_mesh(..) when loading keyname: " << name << std::endl;
		errorlogger("ERROR: Error propogation from load_binary_mesh(..) when loading keyname: ", name.c_str());
		return false;
	}

	base_context->material = manager.load_material(material_name);

	base_context->num_vertices = vertices.size();

	glGenVertexArrays(1, &(base_context->VAO));
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
  
	glBindVertexArray(base_context->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), 
				 &vertices[0], GL_STATIC_DRAW);/* TODO::CHANGE STATIC DRAW?? */

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), 
				 &indices[0], GL_STATIC_DRAW);

	/* Position attribute */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	
	/* Normal attribute */
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	/* TexCoord attribute */
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex_coords));

	/* TODO:: Make conditional after vertex struct split */
	/* Bone ID attribute */
	glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bone_ids));

    /* Bone weight attribute */
    glEnableVertexAttribArray(4); 
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bone_weights));

	/* Unbind */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); 

	/* Check for errors */
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to load mesh from file with name: " << name << std::endl;
		errorlogger("ERROR: Failed to load mesh from file with name: ", name.c_str());
		return false;
	}

	if (base_context->material && (base_context->material->is_complete())) {
		/* yolo */
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Using base color due to incomplete material: " << material_name << std::endl;
		errorlogger("ERROR: WARNING: Using base color due to incomplete material: ", material_name.c_str());
		switch (base_context->shader_type) {
			case GEOMETRY_ANIMATED:
				base_context->shader_type = GEOMETRY_ANIMATED_COLORED;
				break;
			case GEOMETRY_STATIC:
				base_context->shader_type = GEOMETRY_STATIC_COLORED;
				break;
			default:
				std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Invalid shader type in context of mesh: " << name << std::endl;
				errorlogger("ERROR: Invalid shader type in context of mesh: ", name.c_str());
				return false;
		}
	}

	return true;
}

bool Mesh::add_base_to_context(Rendering_context& context)const{
	Base_render_context_weak weak_context = base_context;
	context.base_contexts.push_back(weak_context);
	return true;
}

void Mesh::free_mesh(){
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &(base_context->VAO));

	/* Check for errors */
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to free mesh!" << std::endl;
		errorlogger("ERROR: Failed to free mesh!");
		exit(EXIT_FAILURE);
	}
}