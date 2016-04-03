#include "mesh.h"

Mesh::Mesh(){
	VBO = 0;
	EBO = 0;
	object_color = {1.0f, 1.0f, 1.0f, 1.0f};
	material = nullptr;

	mesh_in_renderer = false;

	base_context = std::make_shared<Rendering_context>();
	base_context->VAO = 0;
	base_context->render_mode = GL_FILL;
	base_context->primitive_type = GL_TRIANGLES;
	base_context->shader_type = GEOMETRY_STATIC_COLORED;
	base_context->render_elements = true;
	base_context->setup_base_uniforms = [&](const Shader_ptr& shader) {
		switch (base_context->shader_type) {
		case GEOMETRY_ANIMATED:
			if (!material->use(shader)) {
				std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to use material for mesh: " << name << std::endl;
				errorlogger("ERROR: Failed to use material for mesh: ", name.c_str());
				return false;
			}
			break;
		case GEOMETRY_ANIMATED_COLORED:
			glUniform4fv(shader->load_uniform_location("object_color"), 1, (float*)&(object_color));
			break;
		case GEOMETRY_STATIC:
			if (!material->use(shader)) {
				std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to use material for mesh: " << name << std::endl;
				errorlogger("ERROR: Failed to use material for mesh: ", name.c_str());
				return false;
			}
			break;
		case GEOMETRY_STATIC_COLORED:
			glUniform4fv(shader->load_uniform_location("object_color"), 1, (float*)&(object_color));
			break;
		case GEOMETRY_LINES:
			break;
		default:
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Unknown shader type when rendering mesh!" << std::endl;
			errorlogger("ERROR: Unknown shader type when rendering mesh!");
			return false;
		}

		return true;
	};
}

Mesh::Mesh(const glm::vec4& color){
	VBO = 0;
	EBO = 0;
	object_color = color;
	material = nullptr;

	mesh_in_renderer = false;

	base_context = std::make_shared<Rendering_context>();
	base_context->VAO = 0;
	base_context->render_mode = GL_FILL;
	base_context->primitive_type = GL_TRIANGLES;
	base_context->shader_type = GEOMETRY_STATIC_COLORED;
	base_context->render_elements = true;
	base_context->setup_base_uniforms = [&](const Shader_ptr& shader) {
		switch (base_context->shader_type) {
		case GEOMETRY_ANIMATED:
			if (!material->use(shader)) {
				std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to use material for mesh: " << name << std::endl;
				errorlogger("ERROR: Failed to use material for mesh: ", name.c_str());
				return false;
			}
			break;
		case GEOMETRY_ANIMATED_COLORED:
			glUniform4fv(shader->load_uniform_location("object_color"), 1, (float*)&(object_color));
			break;
		case GEOMETRY_STATIC:
			if (!material->use(shader)) {
				std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to use material for mesh: " << name << std::endl;
				errorlogger("ERROR: Failed to use material for mesh: ", name.c_str());
				return false;
			}
			break;
		case GEOMETRY_STATIC_COLORED:
			glUniform4fv(shader->load_uniform_location("object_color"), 1, (float*)&(object_color));
			break;
		case GEOMETRY_LINES:
			break;
		default:
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Unknown shader type when rendering mesh!" << std::endl;
			errorlogger("ERROR: Unknown shader type when rendering mesh!");
			return false;
		}

		return true;
	};
}

Mesh::~Mesh(){
	if (!free_mesh()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to delete mesh buffers!" << std::endl;
		errorlogger("FATAL ERROR: Failed to delete mesh buffers!");
		exit(EXIT_FAILURE);
	}
}

bool Mesh::load_binary_mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::string& material_name){
	std::string mesh_path = Utility_vars::folder_path + MESH_DATA_PATH + name + ".mesh";

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

bool Mesh::load_base_box(std::vector<Vertex>& vertices, 
				std::vector<GLuint>& indices) {
	base_context->shader_type = GEOMETRY_STATIC;
	std::vector<glm::vec3> cube_v_positions = {
		glm::vec3(-1.0, -1.0,  1.0),
		glm::vec3(1.0, -1.0,  1.0),
		glm::vec3(1.0,  1.0,  1.0),
		glm::vec3(-1.0,  1.0,  1.0),
		glm::vec3(-1.0, -1.0, -1.0),
		glm::vec3(1.0, -1.0, -1.0),
		glm::vec3(1.0,  1.0, -1.0),
		glm::vec3(-1.0,  1.0, -1.0),
	};

	std::vector<glm::vec3> cube_normals = {
		glm::vec3(-1.0, -1.0,  1.0),
		glm::vec3(1.0, -1.0,  1.0),
		glm::vec3(1.0,  1.0,  1.0),
		glm::vec3(-1.0,  1.0,  1.0),
		glm::vec3(-1.0, -1.0, -1.0),
		glm::vec3(1.0, -1.0, -1.0),
		glm::vec3(1.0,  1.0, -1.0),
		glm::vec3(-1.0,  1.0, -1.0),
	};

	GLushort cube_elements[] = {
		// front
		0, 1, 2, 2, 3, 0,
		// top
		1, 5, 6, 6, 2, 1,
		// back
		7, 6, 5, 5, 4, 7,
		// bottom
		4, 0, 3, 3, 7, 4,
		// left
		4, 5, 1, 1, 0, 4,
		// right
		3, 2, 6, 6, 7, 3,
	};

	Vertex vertex;
	for (GLuint i = 0; i < cube_v_positions.size(); ++i) {
		vertex.position = cube_v_positions[i];
		vertex.normal = cube_normals[i];
		vertex.tex_coords = {0.0f, 0.0f};
		vertices.push_back(vertex);
	}

	for (GLushort i = 0; i < 36; ++i) {
		indices.push_back(cube_elements[i]);
	}

	return true;
}

bool Mesh::load_base_line(std::vector<Vertex>& vertices) {
	base_context->shader_type = GEOMETRY_LINES;
	std::vector<glm::vec3> line_v_positions = {
		glm::vec3(0.0, 0.0,  0.0),
		glm::vec3(1.0, 0.0,  0.0),
	};

	Vertex vertex;
	for (GLuint i = 0; i < line_v_positions.size(); ++i) {
		vertex.position = line_v_positions[i];
		vertices.push_back(vertex);
	}

	base_context->render_elements = false;
	base_context->primitive_type = GL_LINES;

	return true;
}

bool Mesh::load_from_file(Resource_manager& manager, const std::string& name){
	if( VBO != 0 ){
		free_mesh();
	}

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::string material_name = "";

	if (name == Mesh_vars::BOX) {
		if (!load_base_box(vertices, indices)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to load base box mesh!" << std::endl;
			errorlogger("ERROR: Failed to load base box mesh!");
			return false;
		}
	}

	else if (name == Mesh_vars::LINE) {
		if (!load_base_line(vertices)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to load base box mesh!" << std::endl;
			errorlogger("ERROR: Failed to load base box mesh!");
			return false;
		}
	}

	else if (!load_binary_mesh(name, vertices, indices, material_name)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to load binary mesh with name: " << name << std::endl;
		errorlogger("ERROR: Failed to load binary mesh with name: ", name.c_str());
		return false;
	}

	if (!material_name.empty()) {
		material = manager.load_material(material_name);
	}
	
	if (material && (material->is_complete())) {
		/* yolo */
	}
	else{
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Using base color due to incomplete material: " << material_name << std::endl;
		errorlogger("WARNING: Using base color due to incomplete material: ", material_name.c_str());
		switch (base_context->shader_type) {
			case GEOMETRY_ANIMATED:
				base_context->shader_type = GEOMETRY_ANIMATED_COLORED;
				break;
			case GEOMETRY_STATIC:
				base_context->shader_type = GEOMETRY_STATIC_COLORED;
				break;
			case GEOMETRY_LINES:
				break;
			default:
				std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Invalid shader type in context of mesh: " << name << std::endl;
				errorlogger("ERROR: Invalid shader type in context of mesh: ", name.c_str());
				return false;
		}
	}

	if (indices.size() > 0) {
		base_context->num_vertices = indices.size();
	}
	else{
		base_context->num_vertices = vertices.size();
	}

	glGenVertexArrays(1, &(base_context->VAO));
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
  
	glBindVertexArray(base_context->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), 
				 &vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), 
				 &indices[0], GL_DYNAMIC_DRAW);

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

	return true;
}

bool Mesh::free_mesh(){
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &(base_context->VAO));

	if(check_ogl_error()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to free mesh!" << std::endl;
		errorlogger("ERROR: Failed to free mesh!");
		return false;
	}

	return true;
}

Rendering_context_weak Mesh::get_context()const {
	Rendering_context_weak weak_context = base_context;
	return weak_context;
}

bool Mesh::add_uniform_setup(const std::string& modelname, 
					std::function<GLboolean(const Shader_ptr& shader, GLuint instance)> expression) {
	if (base_context->instance_uniform_setups.find(modelname) == base_context->instance_uniform_setups.end()) {
		base_context->instance_uniform_setups[modelname] = expression;
		return true;
	}
	std::cout << __FILE__ << ":" << __LINE__  << ": " << "ERROR: Failed to add lambda expression to mesh context, keyname: " << modelname << std::endl;
	errorlogger("ERROR: Failed to add lambda expression to mesh context, keyname: ", modelname.c_str());
	return false;
}

bool Mesh::remove_uniform_setup(const std::string& modelname) {
	if (base_context->instance_uniform_setups.find(modelname) != base_context->instance_uniform_setups.end()) {
		base_context->instance_uniform_setups.erase(modelname);
		return true;
	}

	return false;
}

bool Mesh::add_context_to_renderer(Renderer& renderer){
	if (mesh_in_renderer == false) {
		mesh_in_renderer = true;
		if (!renderer.add_context(base_context)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to add mesh context to renderer for mesh: " << name << std::endl;
			errorlogger("ERROR: Failed to add mesh context to renderer for mesh:", name.c_str());
			return false;
		}
	}
	else{
		SDL_Log("Mesh already in renderer: %s", name.c_str());
	}

	return true;
}

bool Mesh::setup_base_uniforms(const Shader_ptr& shader){
	if (!base_context->setup_base_uniforms(shader)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to setup base uniforms for mesh: " << name << std::endl;
		errorlogger("ERROR: Failed to setup base uniforms for mesh: ", name.c_str());
		return false;
	}

	return true;
}

Rendering_context_ptr Mesh::get_main_context()const{
	return base_context;
}