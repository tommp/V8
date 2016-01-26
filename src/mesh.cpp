#include "headers/mesh.h"

Mesh::Mesh(){
	VBO = 0;
	VAO = 0;
	EBO = 0;
}

void Mesh::render_mesh(const glm::vec3& position){

}

bool Mesh::load_from_file(const std::string& name){

	vector<Vertex> vertices;
	vector<GLuint> indices;

	if (!load_binary_mesh(name, vertices, indices)) {
		errorlogger("ERROR: Error propogation from load_binary_mesh(..) when loading keyname: ", name.c_str());
		std::cout << "ERROR: Error propogation from load_binary_mesh(..) when loading keyname: " << name.c_str() << std::endl;
		return false;
	}

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
	
	/* Color */
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	
	/* TexCoord attribute */
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex_coords));

	/* Normal attribute */
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	/* Unbind */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); 

	return true;
}

void Mesh::free_mesh(){

}