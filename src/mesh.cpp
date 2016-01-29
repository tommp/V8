#include "mesh.h"

Mesh::Mesh(){
	VBO = 0;
	VAO = 0;
	EBO = 0;
	shader = nullptr;
}

Mesh::Mesh(Shader_ptr init_shader){
	VBO = 0;
	VAO = 0;
	EBO = 0;
	shader = init_shader;
}

bool Mesh::load_from_file(const std::string& name){

	/* Get rid of preexisting mesh */
	if( VBO != 0 ){
		free_mesh();
	}

	vector<Vertex> vertices;
	vector<GLuint> indices;

	if (!load_binary_mesh(name, vertices, indices)) {
		errorlogger("ERROR: Error propogation from load_binary_mesh(..) when loading keyname: ", name.c_str());
		std::cout << "ERROR: Error propogation from load_binary_mesh(..) when loading keyname: " << name.c_str() << std::endl;
		return false;
	}

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

	/* Check for errors */
	if(check_ogl_error()){
		errorlogger("ERROR: Failed to load mesh from file with name: ", name.c_str());
		std::cout << "ERROR: Failed to load mesh from file with name: " << name << std::endl;
		exit(EXIT_FAILURE);
	}

	return true;
}

void Mesh::render_mesh(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, GLfloat rotate){
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position));  

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z)); 
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.5f * size.z));

    model = glm::scale(model, glm::vec3(size)); 
  
    shader->set_matrix4("model", model);
    shader->set_vector4f("color", color);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    glBindVertexArray(0);
}

void Mesh::free_mesh(){
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	/* Check for errors */
	if(check_ogl_error()){
		errorlogger("ERROR: Failed to free mesh!");
		std::cout << "ERROR: Failed to free mesh!" << std::endl;
		exit(EXIT_FAILURE);
	}
}