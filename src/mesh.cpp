#include "mesh.h"

Mesh::Mesh(){
	VBO = 0;
	VAO = 0;
	EBO = 0;
}

Mesh::~Mesh(){
	free_mesh();
}

bool Mesh::load_binary_mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<GLuint>& indices){
	std::ifstream contentf (MESH_DATA_PATH, std::ios::binary);

	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for mesh data!" << std::endl;
		errorlogger("ERROR: Failed to open content file for mesh data!");
		return false;
	}

	if (ENGINE_MESHES.find(name) == ENGINE_MESHES.end()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: No image in mesh_map with keyname: " << name << std::endl;
		errorlogger("ERROR: No image in mesh_map with keyname: ", name.c_str());
		return false;
	}

	GLuint datapos = ENGINE_MESHES.find(name)->second.first;

	contentf.seekg(datapos);

	Vertex vertex;
	GLuint index;
	unsigned int vsize = 0;
	unsigned int isize = 0;

	contentf.read(reinterpret_cast<char *>(&vsize), sizeof(unsigned int));
	contentf.read(reinterpret_cast<char *>(&isize), sizeof(unsigned int));
	for (unsigned int i = 0; i < vsize; ++i) {
		contentf.read(reinterpret_cast<char *>(&vertex), sizeof(Vertex));
		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < isize; ++i) {
		contentf.read(reinterpret_cast<char *>(&index), sizeof(GLuint));
		indices.push_back(index);
	}

	contentf.close();

	return true;
}

bool Mesh::load_from_file(Resource_manager& resource_manager, const std::string& name){

	/* Get rid of preexisting mesh */
	if( VBO != 0 ){
		free_mesh();
	}

	vector<Vertex> vertices;
	vector<GLuint> indices;

	if (!load_binary_mesh(name, vertices, indices)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Error propogation from load_binary_mesh(..) when loading keyname: " << name.c_str() << std::endl;
		errorlogger("ERROR: Error propogation from load_binary_mesh(..) when loading keyname: ", name.c_str());
		return false;
	}

	material = resource_manager.load_material(ENGINE_MESHES.find(name)->second.second);

	num_vertices = vertices.size();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
  
	glBindVertexArray(VAO);
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

	/* Unbind */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); 

	/* Check for errors */
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to load mesh from file with name: " << name << std::endl;
		errorlogger("ERROR: Failed to load mesh from file with name: ", name.c_str());
		exit(EXIT_FAILURE);
	}

	return true;
}

void Mesh::render_mesh(const Renderer& renderer, const glm::vec3& position, const glm::vec3& size, const glm::vec3& direction){
	renderer.render_geometry(VAO, num_vertices, material, position, size, direction);
}

void Mesh::free_mesh(){
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	/* Check for errors */
	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to free mesh!" << std::endl;
		errorlogger("ERROR: Failed to free mesh!");
		exit(EXIT_FAILURE);
	}
}