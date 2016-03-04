#include "utility.h"

bool write_string_to_binary_file(std::ofstream& fstream, const std::string& string) {
	GLuint string_length = string.length();
	if (string_length == 0) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Empty string input!" << string << std::endl;
		errorlogger("ERROR: Empty string input!");
		exit(EXIT_FAILURE);
	}

	if (string_length > MAX_FILENAME_LENGTH) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: String name too long, clipping it: " << string << std::endl;
		string_length = MAX_FILENAME_LENGTH;
	}
	fstream.write(reinterpret_cast<const char *>(&string_length), sizeof(GLuint));
	for (int i = 0; i < string_length; ++i) {
		fstream.write(reinterpret_cast<const char *>(&(string[i])), sizeof(char));
	}

	return true;
}

std::string read_string_from_binary_file(std::ifstream& fstream){
	GLuint string_length;
	fstream.read(reinterpret_cast<char *>(&string_length), sizeof(GLuint));
	if (string_length == 0) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Empty string input!" << std::endl;
		errorlogger("ERROR: Empty string input!");
		exit(EXIT_FAILURE);
	}
	if (string_length > MAX_FILENAME_LENGTH) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: String name too long, clipping it! Length: " << string_length << std::endl;
		string_length = MAX_FILENAME_LENGTH;
	}

	char string[MAX_FILENAME_LENGTH];
	for (int i = 0; i < string_length; ++i) {
		fstream.read(reinterpret_cast<char *>(&(string[i])), sizeof(char));
	}

	string[string_length] = '\0';

	std::string result = string;
	return result;
}

std::vector<std::string> glob(const std::string& path){
    glob_t glob_result;
    glob(path.c_str(), GLOB_TILDE, NULL, &glob_result);
    std::vector<std::string> ret;
    for(GLuint i=0; i<glob_result.gl_pathc; i++){
        ret.push_back(std::string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return ret;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
    	if (!item.empty()){
    		elems.push_back(item);
    	}
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	if(s == ""){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot split empty string!" << std::endl;
		errorlogger("ERROR: Cannot split empty string!");
		exit(EXIT_FAILURE);
	}
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

bool convert_all_models(){
	std::string source_path_mask = RAW_MODEL_DATA_PATH;
	source_path_mask += "*";
	std::vector<std::string> sources = glob(source_path_mask.c_str());
	std::vector<std::string> targets;
	for (auto path : sources) {
		char token = '/';
		if (path.find_first_of('\\') != std::string::npos){
			token = '\\';
		}
		std::vector<std::string> tokens = split(path, token);
		std::string filename = tokens.back();
		std::vector<std::string> file_tokens = split(filename, '.');
		std::string new_target = (MODEL_DATA_PATH + file_tokens[0] + ".model");
		targets.push_back(new_target);
	}

	if(sources.size() != targets.size()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to convert models, size error in vectors!" << std::endl;
		errorlogger("ERROR: Failed to convert models, size error in vectors!");
		return false;
	}
	
	for(GLuint i = 0; i < sources.size(); ++i) {
		if (!convert_model_file(sources[i], targets[i])){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to convert model: " << sources[i] << std::endl;
			errorlogger("ERROR: Failed to convert model: ", sources[i].c_str());
		}
	}
	return true;
}

bool convert_all_images(){
	std::string source_path_mask = IMAGE_DATA_PATH;
	source_path_mask += "*";
	std::vector<std::string> sources = glob(source_path_mask.c_str());
	std::vector<std::string> targets;
	for (auto path : sources) {
		std::vector<std::string> tokens = split(path, '/');
		std::string filename = tokens.back();
		std::vector<std::string> file_tokens = split(filename, '.');
		std::string new_target = (TEXTURE_DATA_PATH + file_tokens[0] + ".tex");
		targets.push_back(new_target);
	}

	if(sources.size() != targets.size()) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to convert images, size error in vectors!" << std::endl;
		errorlogger("ERROR: Failed to convert images, size error in vectors!");
		return false;
	}
	
	for(GLuint i = 0; i < sources.size(); ++i) {
		if (!convert_image_file(sources[i], targets[i])){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to convert image: " << sources[i] << std::endl;
			errorlogger("ERROR: Failed to convert image: ", sources[i].c_str());
		}
	}
	return true;
}

bool convert_image_file(const std::string& source_path, const std::string& target_path){
	GLint width, height, channels;
	unsigned char* image = SOIL_load_image(source_path.c_str(), &width, &height, &channels, SOIL_LOAD_RGB);
	if (!image) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: SOIL failed to convert image: " << source_path << std::endl;
		errorlogger("ERROR: SOIL failed to convert image: ", source_path.c_str());
		return false;
	}

	if (!store_binary_texture(target_path.c_str(), image, width, height, channels, SOIL_LOAD_RGB)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to store target texture: " << target_path << std::endl;
		errorlogger("ERROR: Failed to store target texture: ", target_path.c_str());
	}

	return true;
}

bool store_binary_texture(const std::string& path, 
							unsigned char* image, 
							GLuint width, 
							GLuint height, 
							GLuint channels, 
							GLint format){
	std::ofstream contentf (path.c_str(), std::ios::binary);
	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for storing texture data!" << std::endl;
		errorlogger("ERROR: Failed to open content file for storing texture data!");
		return false;
	}

	contentf.write(reinterpret_cast<const char *>(&width), sizeof(GLuint));
	contentf.write(reinterpret_cast<const char *>(&height), sizeof(GLuint));
	contentf.write(reinterpret_cast<const char *>(&channels), sizeof(GLuint));
	contentf.write(reinterpret_cast<const char *>(&format), sizeof(GLint));
	contentf.write(reinterpret_cast<const char *>(&(image[0])), sizeof(unsigned char) * width * height * channels);

	contentf.close();

	return true;
}

void waitForEvent(){
	SDL_Event event;
	bool done = false;
	while((!done) && (SDL_WaitEvent(&event))) {
		switch(event.type) {
	
			case SDL_KEYDOWN:
				done = true;
				break;

			case SDL_QUIT:
				done = true;
				break;
				
			default:
				break;
		} 	
	}
}

void print_framebuffer_error_in_fucking_english(){
	switch(glCheckFramebufferStatus(GL_FRAMEBUFFER)){
		case GL_FRAMEBUFFER_UNDEFINED:
			std::cout <<  "FRAMEBUFFER ERROR: Default framebuffer does not exist." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout <<  "FRAMEBUFFER ERROR:  Framebuffer attachment point(s) are framebuffer incomplete." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout <<  "FRAMEBUFFER ERROR: Missing attachment." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			std::cout <<  "FRAMEBUFFER ERROR: Incomplete draw buffer." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			std::cout <<  "FRAMEBUFFER ERROR: Incomplete read buffer." << std::endl;
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout <<  "FRAMEBUFFER ERROR: Combination of internal formats of the attached images violates an implementation-dependent set of restrictions." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			std::cout <<  "FRAMEBUFFER ERROR: Invalid values for samples." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			std::cout <<  "FRAMEBUFFER ERROR: Incomplete layers." << std::endl;
			break;
		default:
			std::cout <<  "FRAMEBUFFER ERROR: Unknown error!" << std::endl;
			break;
	}
}

const char* gl_error_string(GLenum err){
  	switch(err) {
		case GL_INVALID_ENUM: return "Invalid Enum";
		case GL_INVALID_VALUE: return "Invalid Value";
		case GL_INVALID_OPERATION: return "Invalid Operation";
		case GL_STACK_OVERFLOW: return "Stack Overflow";
		case GL_STACK_UNDERFLOW: return "Stack Underflow";
		case GL_OUT_OF_MEMORY: return "Out of Memory";
		case GL_TABLE_TOO_LARGE: return "Table too Large";
		default: return "Unknown Error";
	}
}

int check_ogl_error(){
	GLenum gl_error;
	int retCode = 0;

	gl_error = glGetError();

	while (gl_error != GL_NO_ERROR){
		errorlogger("ERROR: glError: ", gl_error_string(gl_error));
		std::cout << "ERROR: glError: " << gl_error_string(gl_error) << std::endl;
		gl_error = glGetError();
		retCode = 1;
	}

	return retCode;
}




//===============================

bool convert_model_file(const std::string& source_path, const std::string& target_path){
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(source_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Assimp failed to load model: " << importer.GetErrorString() << std::endl;
		errorlogger("ERROR: Assimp failed to load model: ", importer.GetErrorString());
		return false;
	}

	std::vector<std::string> mesh_names;

	std::string filename = split(target_path, '/').back();
	std::string modelname = split(filename, '.')[0];

	process_node(scene->mRootNode, scene, mesh_names, modelname);

	/*
	1. Open model file
	2. Write mesh count
	loop:
		1. Write mesh name length
		2. Write mesh name
	*/

	std::ofstream contentf (target_path.c_str(), std::ios::binary);
	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for storing model data: " << target_path << std::endl;
		errorlogger("ERROR: Failed to open content file for storing model data: ", target_path.c_str());
		return false;
	}

	GLuint mesh_count = mesh_names.size();
	contentf.write(reinterpret_cast<const char *>(&mesh_count), sizeof(GLuint));

	for (auto mesh_name : mesh_names) {

		std::cout << "Converting: " << mesh_name << "..." << std::endl;
		write_string_to_binary_file(contentf, mesh_name);
		std::cout << "Done!\n" << std::endl;
	}
	contentf.close();

	return true;
}

void process_node(aiNode* node, 
					const aiScene* scene, 
					std::vector<std::string>& mesh_names, 
					const std::string& modelname){
	for(GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
		mesh_names.push_back(process_mesh(mesh, scene, modelname, i));	

	}

	for(GLuint i = 0; i < node->mNumChildren; i++) {
		process_node(node->mChildren[i], scene, mesh_names, modelname);
	}
}

std::string process_mesh(aiMesh* mesh, 
							const aiScene* scene, 
							const std::string modelname, 
							GLuint meshnumber){
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::string material_name;
	std::string diffuse_name;
	std::string specular_name;

	GLuint true_bool = 1;
	GLuint false_bool = 0;

	for(GLuint i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 pos_vector;

		pos_vector.x = mesh->mVertices[i].x;
		pos_vector.y = mesh->mVertices[i].y;
		pos_vector.z = mesh->mVertices[i].z;
		vertex.position = pos_vector;

		glm::vec3 norm_vector;
		if (mesh->mNormals) {
			norm_vector.x = mesh->mNormals[i].x;
			norm_vector.y = mesh->mNormals[i].y;
			norm_vector.z = mesh->mNormals[i].z;
			vertex.normal = norm_vector;
		}

		if(mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			
			vec.x = mesh->mTextureCoords[0][i].x; 
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.tex_coords = vec;
		}
		else{
			vertex.tex_coords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}
	
	for(GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for(GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	if(mesh->mMaterialIndex > 0) {
		aiMaterial* new_material = scene->mMaterials[mesh->mMaterialIndex];

		aiString name;
		new_material->Get(AI_MATKEY_NAME,name);

		material_name = name.data;
		diffuse_name = load_material_texture(new_material, aiTextureType_DIFFUSE, "texture_diffuse");
		specular_name = load_material_texture(new_material, aiTextureType_SPECULAR, "texture_specular");

	}

	std::string meshname = modelname + "_" + std::to_string(meshnumber);

	/*
	1. Open material file
	1.5. Write contain bools
	2. Write diffuse name length
	3. Write diffuse name
	4. Write specular name length
	5. Write specular name

	*/

	std::string material_path = MATERIAL_DATA_PATH + material_name + ".mat";

	std::ofstream contentf (material_path.c_str(), std::ios::binary);
	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for storing material data: " << material_path << std::endl;
		errorlogger("ERROR: Failed to open content file for storing material data: ", material_path.c_str());
		exit(EXIT_FAILURE);
	}

	if (!diffuse_name.empty()){
		contentf.write(reinterpret_cast<const char *>(&true_bool), sizeof(GLuint));
		write_string_to_binary_file(contentf, diffuse_name);
	}
	else{
		contentf.write(reinterpret_cast<const char *>(&false_bool), sizeof(GLuint));
	}

	if (!specular_name.empty()){
		contentf.write(reinterpret_cast<const char *>(&true_bool), sizeof(GLuint));
		write_string_to_binary_file(contentf, specular_name);
	}
	else{
		contentf.write(reinterpret_cast<const char *>(&false_bool), sizeof(GLuint));
	}

	contentf.close();

	/*
	1. Open mesh file
	2. Write material name length
	3. Write material name
	4. Write num vertices
	5. Write num indices

	loop:
		1. Write vertices
	loop:
		1. Write indices

	*/

	std::string mesh_path = MESH_DATA_PATH + meshname + ".mesh";

	contentf.open(mesh_path.c_str(), std::ios::binary);
	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for storing mesh data: " << mesh_path << std::endl;
		errorlogger("ERROR: Failed to open content file for storing mesh data: ", mesh_path.c_str());
		exit(EXIT_FAILURE);
	}

	if (!material_name.empty()){
		contentf.write(reinterpret_cast<const char *>(&true_bool), sizeof(char));
		write_string_to_binary_file(contentf, material_name);
	}
	else{
		contentf.write(reinterpret_cast<const char *>(&false_bool), sizeof(char));
	}
	
	GLuint vsize = vertices.size();
	GLuint isize = indices.size();

	contentf.write(reinterpret_cast<const char *>(&vsize), sizeof(GLuint));
	contentf.write(reinterpret_cast<const char *>(&isize), sizeof(GLuint));
	for (auto vertex : vertices) {
		contentf.write(reinterpret_cast<const char *>(&vertex), sizeof(Vertex));
	}
	for (auto index : indices) {
		contentf.write(reinterpret_cast<const char *>(&index), sizeof(GLuint));
	}
	
	return meshname;
}

std::vector<std::string> load_material_textures(aiMaterial* mat, 
												aiTextureType type, 
												const std::string& typeName){
	std::vector<std::string> textures;
	for(GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		std::string temp (str.data);
		temp.replace(temp.end() - 4, temp.end(), "");

		textures.push_back(temp);
	}
	return textures;
}

std::string load_material_texture(aiMaterial* mat, 
									aiTextureType type, 
									const std::string& typeName){
	std::string texture;
	if(mat->GetTextureCount(type) >= 1){
		aiString str;
		mat->GetTexture(type, 0, &str);

		std::string temp (str.data);
		
		if (temp.empty()){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to convert aistring to string!" << std::endl;
			errorlogger("ERROR: Failed to convert aistring to string!");
			exit(EXIT_FAILURE);
		}
		else{
			char token = '/';
			if (temp.find_first_of('\\') != std::string::npos){
				token = '\\';
			}
			std::string filename = split(temp, token).back();
			std::string tex_name = split(filename, '.')[0];
			texture = tex_name;
		}
	}
	return texture;
}