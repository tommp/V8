#include "utility.h"

inline GLboolean file_exists(const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

GLboolean write_quaternion_to_binary_file(std::ofstream& contentf, const aiQuatKey& quaternion){
	GLdouble key_time = quaternion.mTime;
	aiQuaternion raw_quat = quaternion.mValue;

	contentf.write(reinterpret_cast<const char *>(&key_time), sizeof(GLdouble));

	contentf.write(reinterpret_cast<const char *>(&(raw_quat.x)), sizeof(GLfloat));
	contentf.write(reinterpret_cast<const char *>(&(raw_quat.y)), sizeof(GLfloat));
	contentf.write(reinterpret_cast<const char *>(&(raw_quat.z)), sizeof(GLfloat));
	contentf.write(reinterpret_cast<const char *>(&(raw_quat.w)), sizeof(GLfloat));
	return true;

}

GLboolean write_vector_to_binary_file(std::ofstream& contentf, const aiVectorKey& vector){
	GLdouble key_time = vector.mTime;
	aiVector3D raw_vec = vector.mValue;

	contentf.write(reinterpret_cast<const char *>(&key_time), sizeof(GLdouble));

	contentf.write(reinterpret_cast<const char *>(&(raw_vec.x)), sizeof(GLfloat));
	contentf.write(reinterpret_cast<const char *>(&(raw_vec.y)), sizeof(GLfloat));
	contentf.write(reinterpret_cast<const char *>(&(raw_vec.z)), sizeof(GLfloat));
	return true;
}

GLboolean read_quaternion_from_binary_file(std::ifstream& contentf, std::pair<GLdouble, glm::fquat>& key_quaternion){
	contentf.read(reinterpret_cast<char *>(&(key_quaternion.first)), sizeof(GLdouble));

	contentf.read(reinterpret_cast<char *>(&(key_quaternion.second.x)), sizeof(GLfloat));
	contentf.read(reinterpret_cast<char *>(&(key_quaternion.second.y)), sizeof(GLfloat));
	contentf.read(reinterpret_cast<char *>(&(key_quaternion.second.z)), sizeof(GLfloat));
	contentf.read(reinterpret_cast<char *>(&(key_quaternion.second.w)), sizeof(GLfloat));
	return true;

}

GLboolean read_vector_from_binary_file(std::ifstream& contentf, std::pair<GLdouble, glm::vec3>& key_vector){
	contentf.read(reinterpret_cast<char *>(&(key_vector.first)), sizeof(GLdouble));

	contentf.read(reinterpret_cast<char *>(&(key_vector.second.x)), sizeof(GLfloat));
	contentf.read(reinterpret_cast<char *>(&(key_vector.second.y)), sizeof(GLfloat));
	contentf.read(reinterpret_cast<char *>(&(key_vector.second.z)), sizeof(GLfloat));
	return true;
}

GLboolean write_string_to_binary_file(std::ofstream& fstream, const std::string& string) {
	GLuint string_length = string.length();
	if (string_length == 0) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Writing empty string!" << std::endl;
		errorlogger("WARNING: Writing empty string!");
		fstream.write(reinterpret_cast<const char *>(&string_length), sizeof(GLuint));
		return false;
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

bool read_string_from_binary_file(std::ifstream& fstream, std::string& data){
	GLuint string_length;
	fstream.read(reinterpret_cast<char *>(&string_length), sizeof(GLuint));
	if (string_length == 0) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Empty string read!" << std::endl;
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

	data = string;
	return true;
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

GLboolean convert_all_models(){
	std::vector<std::string> sources;
	std::string source_path_mask = RAW_MODEL_DATA_PATH;
	source_path_mask += "*";
	std::vector<std::string> filetypes = {".obj", ".3ds", ".ms3d", ".b3d", ".x", "md5mesh", ".blend", ".dae"};
	for (const auto &suffix : filetypes) {
		std::string true_path = source_path_mask + suffix;
		std::vector<std::string> files = glob(true_path.c_str());

		for (const auto &source : files) {
			sources.push_back(source);
		}
	}
	std::vector<std::string> targets;
	for (const auto &path : sources) {
		char token = '/';
		if (path.find_first_of('\\') != std::string::npos){
			token = '\\';
		}
		std::string filename = split(path, token).back();
		std::vector<std::string> file_tokens = split(filename, '.');
		std::string new_target = (MODEL_DATA_PATH + file_tokens[0] + ".model");

		targets.push_back(new_target);
	}

	for(GLuint i = 0; i < sources.size(); ++i) {
		if (file_exists(targets[i])) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Model already exists, skipping conversion: " << targets[i] << std::endl;
			errorlogger("WARNING: Model already exists, skipping conversion: ", targets[i].c_str());
			continue;
		}
		if (!convert_model_file(sources[i], targets[i])){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Failed to convert model: " << sources[i] << std::endl;
			errorlogger("WARNING: Failed to convert model: ", sources[i].c_str());
		}
	}
	return true;
}

GLboolean convert_all_images(){
	std::string source_path_mask = IMAGE_DATA_PATH;
	source_path_mask += "*";
	std::vector<std::string> sources = glob(source_path_mask.c_str());
	std::vector<std::string> targets;
	for (const auto &path : sources) {
		std::vector<std::string> tokens = split(path, '/');
		std::string filename = tokens.back();
		std::vector<std::string> file_tokens = split(filename, '.');
		std::string new_target = (TEXTURE_DATA_PATH + file_tokens[0] + ".tex");
		targets.push_back(new_target);
	}
	
	for(GLuint i = 0; i < sources.size(); ++i) {
		if (file_exists(targets[i])) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Texture already exists, skipping conversion: " << targets[i] << std::endl;
			errorlogger("WARNING: Texture already exists, skipping conversion: ", targets[i].c_str());
			continue;
		}
		if (!convert_image_file(sources[i], targets[i])){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Failed to convert image: " << sources[i] << std::endl;
			errorlogger("WARNING: Failed to convert image: ", sources[i].c_str());
		}
	}
	return true;
}

GLboolean convert_image_file(const std::string& source_path, const std::string& target_path){
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
		return false;
	}

	return true;
}

GLboolean store_binary_texture(const std::string& path, 
							unsigned char* image, 
							GLuint width, 
							GLuint height, 
							GLuint channels, 
							GLint format){
	if (file_exists(path)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to store binary texture, target file exists: " << path << std::endl;
		errorlogger("ERROR: Failed to store binary texture, target file exists: ", path.c_str());
		return false;
	}

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

GLboolean convert_model_file(const std::string& source_path, const std::string& target_path){
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(source_path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Assimp failed to load model: " << importer.GetErrorString() << std::endl;
		errorlogger("ERROR: Assimp failed to load model: ", importer.GetErrorString());
		return false;
	}

	std::vector<std::string> mesh_names;

	std::string filename = split(target_path, '/').back();
	std::string modelname = split(filename, '.')[0];

	process_node(scene->mRootNode, scene, mesh_names);

	if (file_exists(target_path)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to store binary model, target file exists: " << target_path << std::endl;
		errorlogger("ERROR: Failed to store binary model, target file exists: ", target_path.c_str());
		return false;
	}

	std::ofstream contentf (target_path.c_str(), std::ios::binary);
	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to open content file for storing model data: " << target_path << std::endl;
		errorlogger("ERROR: Failed to open content file for storing model data: ", target_path.c_str());
		return false;
	}

	GLuint mesh_count = mesh_names.size();
	contentf.write(reinterpret_cast<const char *>(&mesh_count), sizeof(GLuint));

	for (const auto &mesh_name : mesh_names) {
		std::cout << "Converting: " << mesh_name << "..." << std::endl;
		if (!write_string_to_binary_file(contentf, mesh_name)) {
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Cannot add empty mesh name as model mesh key for: " << target_path << std::endl;
			errorlogger("ERROR: Cannot add empty mesh name as model mesh key for: ", target_path.c_str());
			return false;
		}
		std::cout << "Done!\n" << std::endl;
	}

	if (scene->HasAnimations()) {
		contentf.write(reinterpret_cast<const char *>(&TRUE_BOOL), sizeof(GLuint));
		store_binary_animation_set(scene, modelname);
	}
	else{
		contentf.write(reinterpret_cast<const char *>(&FALSE_BOOL), sizeof(GLuint));
	}

	contentf.close();
	
	return true;
}

void process_node(const aiNode* node, 
					const aiScene* scene, 
					std::vector<std::string>& mesh_names){
	for(GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
		mesh_names.push_back(process_mesh(mesh, scene));	
	}

	for(GLuint i = 0; i < node->mNumChildren; i++) {
		process_node(node->mChildren[i], scene, mesh_names);
	}
}

std::string process_mesh(const aiMesh* mesh, const aiScene* scene){
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::string material_name;

	vertices.resize(mesh->mNumVertices);

	for(GLuint i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		initialize_vertex(vertex);

		glm::vec3 pos_vector;
		pos_vector.x = mesh->mVertices[i].x;
		pos_vector.y = mesh->mVertices[i].y;
		pos_vector.z = mesh->mVertices[i].z;
		vertex.position = pos_vector;

		if (mesh->mNormals) {
			glm::vec3 norm_vector;
			norm_vector.x = mesh->mNormals[i].x;
			norm_vector.y = mesh->mNormals[i].y;
			norm_vector.z = mesh->mNormals[i].z;
			vertex.normal = norm_vector;
		}

		if(mesh->mTextureCoords[0]) {
			glm::vec2 tex_vector;
			tex_vector.x = mesh->mTextureCoords[0][i].x; 
			tex_vector.y = mesh->mTextureCoords[0][i].y;
			vertex.tex_coords = tex_vector;
		}

		vertices[i] = vertex;
	}
	
	for(GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for(GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	std::string meshname = mesh->mName.data;
	if (!store_binary_material(scene, mesh, material_name)){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Failed to store binary material!" << std::endl;
		errorlogger("WARNING: Failed to store binary material!");
	}

	if (scene->HasAnimations()){
		std::unordered_map<std::string, GLuint> bone_map;
		std::vector<glm::mat4> bone_info;
		load_mesh_bones(mesh, bone_map, vertices, bone_info);
		store_binary_mesh(scene, vertices, indices, material_name, meshname, bone_map, bone_info);
	}
	else{
		store_binary_mesh(vertices, indices, material_name, meshname);
	}

	return meshname;
}

void store_binary_mesh(const std::vector<Vertex>& vertices, 
						const std::vector<GLuint>& indices, 
						const std::string& material_name,
						std::string& meshname) {
	std::string mesh_path = MESH_DATA_PATH + meshname + ".mesh";

	/* TODO::Better handling */
	int counter = 0;
	while (file_exists(mesh_path)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Failed to store binary mesh, target file exists: " << mesh_path << std::endl;
		errorlogger("WARNING: Failed to store binary mesh, target file exists: ", mesh_path.c_str());
		
		std::cout<< "Renaming..." << std::endl;
		meshname = meshname + std::to_string(counter);
		mesh_path = MESH_DATA_PATH + meshname + ".mesh";
		++counter;
	}

	std::ofstream contentf(mesh_path.c_str(), std::ios::binary);
	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to open content file for storing mesh data: " << mesh_path << std::endl;
		errorlogger("FATAL ERROR: Failed to open content file for storing mesh data: ", mesh_path.c_str());
		exit(EXIT_FAILURE);
	}

	if (!material_name.empty()){
		contentf.write(reinterpret_cast<const char *>(&TRUE_BOOL), sizeof(char));
		write_string_to_binary_file(contentf, material_name);
	}
	else{
		contentf.write(reinterpret_cast<const char *>(&FALSE_BOOL), sizeof(char));
	}

	GLuint vsize = vertices.size();
	GLuint isize = indices.size();

	contentf.write(reinterpret_cast<const char *>(&vsize), sizeof(GLuint));
	contentf.write(reinterpret_cast<const char *>(&isize), sizeof(GLuint));
	for (const auto &vertex : vertices) {
		contentf.write(reinterpret_cast<const char *>(&vertex), sizeof(Vertex));
	}
	for (const auto &index : indices) {
		contentf.write(reinterpret_cast<const char *>(&index), sizeof(GLuint));
	}

	/* No animation */
	contentf.write(reinterpret_cast<const char *>(&FALSE_BOOL), sizeof(char));
}

void store_binary_mesh(const aiScene* scene,
						const std::vector<Vertex>& vertices, 
						const std::vector<GLuint>& indices, 
						const std::string& material_name,
						std::string& meshname,
						const std::unordered_map<std::string, GLuint>& bone_map,
						const std::vector<glm::mat4>& bone_info) {
	std::string mesh_path = MESH_DATA_PATH + meshname + ".mesh";

	/* TODO::Better handling */
	int counter = 0;
	while (file_exists(mesh_path)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "WARNING: Failed to store binary mesh, target file exists: " << mesh_path << std::endl;
		errorlogger("WARNING: Failed to store binary mesh, target file exists: ", mesh_path.c_str());
		
		std::cout<< "Renaming..." << std::endl;
		meshname = meshname + std::to_string(counter);
		mesh_path = MESH_DATA_PATH + meshname + ".mesh";
		++counter;
	}

	std::ofstream contentf(mesh_path.c_str(), std::ios::binary);
	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to open content file for storing mesh data: " << mesh_path << std::endl;
		errorlogger("FATAL ERROR: Failed to open content file for storing mesh data: ", mesh_path.c_str());
		exit(EXIT_FAILURE);
	}

	if (!material_name.empty()){
		contentf.write(reinterpret_cast<const char *>(&TRUE_BOOL), sizeof(char));
		write_string_to_binary_file(contentf, material_name);
	}
	else{
		contentf.write(reinterpret_cast<const char *>(&FALSE_BOOL), sizeof(char));
	}

	GLuint vsize = vertices.size();
	GLuint isize = indices.size();

	contentf.write(reinterpret_cast<const char *>(&vsize), sizeof(GLuint));
	contentf.write(reinterpret_cast<const char *>(&isize), sizeof(GLuint));
	for (const auto &vertex : vertices) {
		contentf.write(reinterpret_cast<const char *>(&vertex), sizeof(Vertex));
	}
	for (const auto &index : indices) {
		contentf.write(reinterpret_cast<const char *>(&index), sizeof(GLuint));
	}

	/* Animation */
	contentf.write(reinterpret_cast<const char *>(&TRUE_BOOL), sizeof(char));

	aiMatrix4x4 root_trans = scene->mRootNode->mTransformation;
	root_trans.Inverse();
	for (GLuint i = 0; i < 4; ++i){
		for (GLuint j = 0; j < 4; ++j) {
			contentf.write(reinterpret_cast<const char *>(&(root_trans[i][j])), sizeof(float));
		}
	}

	GLuint num_bones = bone_info.size();
	contentf.write(reinterpret_cast<const char *>(&num_bones), sizeof(GLuint));
	for (const auto &matrix : bone_info) {
		for (GLuint x = 0; x < 4; ++x) {
			for (GLuint y = 0; y < 4; ++y) {
				contentf.write(reinterpret_cast<const char *>(&(matrix[x][y])), sizeof(float));
			}
		}
	}

	GLuint num_bone_mappings = bone_map.size();
	contentf.write(reinterpret_cast<const char *>(&num_bone_mappings), sizeof(GLuint));
	for (const auto &mapping : bone_map) {
		if (!write_string_to_binary_file(contentf, mapping.first)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Cannot store nameless bone, key must be named, mesh: " << meshname << std::endl;
			errorlogger("FATAL ERROR: Cannot store nameless bone, key must be named, mesh: ", meshname.c_str());
			exit(EXIT_FAILURE);
		}
		contentf.write(reinterpret_cast<const char *>(&(mapping.second)), sizeof(GLuint));
	}
}

GLboolean store_binary_animation_set(const aiScene* scene, const std::string& modelname){
	std::string anim_set_path = ANIMATION_DATA_PATH + modelname + ".anims";
	std::ofstream contentf_set(anim_set_path.c_str(), std::ios::binary);
	std::vector<std::string> animation_set_nodes;

	/* Write animations */
	GLuint num_animations = scene->mNumAnimations;

	contentf_set.write(reinterpret_cast<const char *>(&num_animations), sizeof(GLuint));
	for (GLuint i = 0; i < num_animations; ++i) {
		std::string anim_name = modelname + "_" + scene->mAnimations[i]->mName.data;

		if (!write_string_to_binary_file(contentf_set, anim_name)){
			std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Cannot store nameless animation, key must be named, model: " << modelname << std::endl;
			errorlogger("FATAL ERROR: Cannot store nameless animation, key must be named, model: ", modelname.c_str());
			exit(EXIT_FAILURE);
		}

		GLdouble duration = scene->mAnimations[i]->mDuration;
		GLdouble ticks_per_second = scene->mAnimations[i]->mTicksPerSecond;
		GLuint num_channels = scene->mAnimations[i]->mNumChannels;

		std::string anim_path = ANIMATION_DATA_PATH + anim_name + ".anim";

		std::ofstream contentf(anim_path.c_str(), std::ios::binary);

		contentf.write(reinterpret_cast<const char *>(&duration), sizeof(GLdouble));
		contentf.write(reinterpret_cast<const char *>(&ticks_per_second), sizeof(GLdouble));
		contentf.write(reinterpret_cast<const char *>(&num_channels), sizeof(GLuint));
		for (GLuint j = 0; j < num_channels; ++j) {
			std::string channel_name = scene->mAnimations[i]->mChannels[j]->mNodeName.data;
			GLboolean node_in_set = false;
			for (const auto &node : animation_set_nodes) {
				if (node == channel_name) {
					node_in_set = true;
					break;
				}
			}

			if (!node_in_set) {
				animation_set_nodes.push_back(channel_name);
			}

			GLuint num_pos_keys = scene->mAnimations[i]->mChannels[j]->mNumPositionKeys;
			GLuint num_rot_keys = scene->mAnimations[i]->mChannels[j]->mNumRotationKeys;
			GLuint num_scale_keys = scene->mAnimations[i]->mChannels[j]->mNumScalingKeys;

			if (!write_string_to_binary_file(contentf, channel_name)){
				std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Cannot store nameless channel, key must be named, model: " << modelname << std::endl;
				errorlogger("FATAL ERROR: Cannot store nameless channel, key must be named, model: ", modelname.c_str());
				exit(EXIT_FAILURE);
			}

			contentf.write(reinterpret_cast<const char *>(&num_pos_keys), sizeof(GLuint));
			for (GLuint k = 0; k < num_pos_keys; ++k) {
				write_vector_to_binary_file(contentf, 
									scene->mAnimations[i]->mChannels[j]->mPositionKeys[k]);
			}

			contentf.write(reinterpret_cast<const char *>(&num_rot_keys), sizeof(GLuint));
			for (GLuint k = 0; k < num_rot_keys; ++k) {
				write_quaternion_to_binary_file(contentf, 
										scene->mAnimations[i]->mChannels[j]->mRotationKeys[k]);
			}

			contentf.write(reinterpret_cast<const char *>(&num_scale_keys), sizeof(GLuint));
			for (GLuint k = 0; k < num_scale_keys; ++k) {
				write_vector_to_binary_file(contentf, 
									scene->mAnimations[i]->mChannels[j]->mScalingKeys[k]);
			
			}
		}

		contentf.close();
	}

	sort(animation_set_nodes.begin(), animation_set_nodes.end());

	for (const auto &node : animation_set_nodes) {
		aiNode* root_candidate_parent = scene->mRootNode->FindNode(node.c_str())->mParent;
		if (root_candidate_parent){
			GLboolean is_skeletal_root = true;
			if(std::find(animation_set_nodes.begin(), animation_set_nodes.end(), root_candidate_parent->mName.data) != animation_set_nodes.end()) {
			    is_skeletal_root = false;
			}

			if (is_skeletal_root) {
				store_ai_node_tree(contentf_set, scene->mRootNode->FindNode(node.c_str()), true, animation_set_nodes);
				break;
			}
		}
		else{
			store_ai_node_tree(contentf_set, scene->mRootNode->FindNode(node.c_str()), true, animation_set_nodes);
			break;
		}
	}

	contentf_set.write(reinterpret_cast<const char *>(&FALSE_BOOL), sizeof(GLuint));

	contentf_set.close();

	return true;
}

void store_ai_node_tree(std::ofstream& contentf, const aiNode* node, GLboolean root, 
						const std::vector<std::string>& bone_names){
	/* To signal a node is coming up */
	contentf.write(reinterpret_cast<const char *>(&TRUE_BOOL), sizeof(GLuint));

	GLuint num_children = node->mNumChildren;

	/* Use the pointers as ID's */
	const void * address = static_cast<const void*>(node);
	void* parent_adress;
	std::stringstream node_ss;
	std::stringstream parent_ss;

	if (!root) {
		parent_adress = static_cast<void*>(node->mParent);
		parent_ss << parent_adress;
	}
	else{
		parent_ss << "ROOT";
	}

	node_ss << address;  
	
	std::string node_id = node_ss.str(); 
	std::string parent_id = parent_ss.str();

	write_string_to_binary_file(contentf, node_id);
	write_string_to_binary_file(contentf, parent_id);

	std::string node_name = node->mName.data;
	write_string_to_binary_file(contentf, node_name);

	for (GLuint x = 0; x < 4; ++x) {
		for (GLuint y = 0; y < 4; ++y) {
			GLfloat index = node->mTransformation[x][y];
			contentf.write(reinterpret_cast<const char *>(&index), sizeof(GLfloat));
		}
	}

	for (GLuint i = 0; i < num_children; ++i) {
		if (std::find(bone_names.begin(), bone_names.end(), node->mChildren[i]->mName.data) != bone_names.end()) {
			store_ai_node_tree(contentf, node->mChildren[i], false, bone_names);
		}
	}
}

GLboolean store_binary_material(const aiScene* scene, const aiMesh* mesh, std::string& material_name) {
	aiMaterial* new_material = scene->mMaterials[mesh->mMaterialIndex];
	std::string material_path;
	std::string diffuse_name;
	std::string specular_name;
	aiString name;

	new_material->Get(AI_MATKEY_NAME,name);

	material_name = name.data;
	diffuse_name = load_material_texture(new_material, aiTextureType_DIFFUSE, "texture_diffuse");
	specular_name = load_material_texture(new_material, aiTextureType_SPECULAR, "texture_specular");

	material_path = MATERIAL_DATA_PATH + material_name + ".mat";

	if (file_exists(material_path)) {
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Failed to store binary material, target file exists: " << material_path << std::endl;
		errorlogger("ERROR: Failed to store binary material, target file exists: ", material_path.c_str());
		return false;
	}

	std::ofstream contentf (material_path.c_str(), std::ios::binary);
	if (!contentf.is_open()){
		std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Failed to open content file for storing material data: " << material_path << std::endl;
		errorlogger("FATAL ERROR: Failed to open content file for storing material data: ", material_path.c_str());
		exit(EXIT_FAILURE);
	}

	if (!diffuse_name.empty()){
		contentf.write(reinterpret_cast<const char *>(&TRUE_BOOL), sizeof(GLuint));
		write_string_to_binary_file(contentf, diffuse_name);
	}
	else{
		contentf.write(reinterpret_cast<const char *>(&FALSE_BOOL), sizeof(GLuint));
	}

	if (!specular_name.empty()){
		contentf.write(reinterpret_cast<const char *>(&TRUE_BOOL), sizeof(GLuint));
		write_string_to_binary_file(contentf, specular_name);
	}
	else{
		contentf.write(reinterpret_cast<const char *>(&FALSE_BOOL), sizeof(GLuint));
	}

	contentf.close();

	return true;
}

/* WARNING::Vertex vector must be sorted according to aimesh indices or shit goes south */
void load_mesh_bones(const aiMesh* mesh, 
						std::unordered_map<std::string, GLuint>& bone_map,
						std::vector<Vertex>& vertices,
						std::vector<glm::mat4>& bone_info){
	for (GLuint i = 0 ; i < mesh->mNumBones; ++i) { 
        GLuint bone_index = 0; 
        std::string bone_name(mesh->mBones[i]->mName.data);

        if (bone_map.find(bone_name) == bone_map.end()) {
            bone_index = bone_info.size();
            glm::mat4 bone_offset; 
            bone_info.push_back(bone_offset);
            bone_map[bone_name] = bone_index;
        }
        else {
            bone_index = bone_map[bone_name];
        }

        aiMatrix4x4 offset_aimatrix = mesh->mBones[i]->mOffsetMatrix;
        glm::mat4 offsetmatrix;

        for (GLuint x = 0; x < 4; ++x) {
        	for (GLuint y = 0; y < 4; ++y) {
        		offsetmatrix[x][y] = offset_aimatrix[x][y];
        	}
        }

        bone_info[bone_index]= offsetmatrix;


        for (GLuint j = 0 ; j < mesh->mBones[i]->mNumWeights; ++j) {
        	/* TODO::Make sure this gets the right vertex id */
            GLuint vertex_id = mesh->mBones[i]->mWeights[j].mVertexId;
            GLfloat weight = mesh->mBones[i]->mWeights[j].mWeight; 
            if (!add_bone_to_vertex(vertices[vertex_id], bone_index)){
            	std::cout << __FILE__ << ":" << __LINE__ << ": " << "FATAL ERROR: Vertex can only be affected by four bones, id overflow in bone: " << bone_name << std::endl;
				errorlogger("FATAL ERROR: Vertex can only be affected by four bones, id overflow in bone: ", bone_name.c_str());
            	exit(EXIT_FAILURE);
            }
            if (!add_weight_to_vertex(vertices[vertex_id], weight)) {
            	std::cout << __FILE__ << ":" << __LINE__ << ": " << "ERROR: Vertex can only be affected by four bones, weight overflow in bone: " << bone_name << std::endl;
				errorlogger("ERROR: Vertex can only be affected by four bones, weight overflow in bone: ", bone_name.c_str());
            	exit(EXIT_FAILURE);
            }
        }
    }
}

/* Not in use, rework it if needed */
std::vector<std::string> load_material_textures(const aiMaterial* mat, 
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

std::string load_material_texture(const aiMaterial* mat, 
									aiTextureType type, 
									const std::string& typeName){
	std::string texture = "";
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

void waitForEvent(){
	SDL_Event event;
	GLboolean done = false;
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

GLint check_ogl_error(){
	GLenum gl_error;
	GLint retCode = 0;

	gl_error = glGetError();

	while (gl_error != GL_NO_ERROR){
		errorlogger("ERROR: glError: ", gl_error_string(gl_error));
		std::cout << "ERROR: glError: " << gl_error_string(gl_error) << std::endl;
		gl_error = glGetError();
		retCode = 1;
	}

	return retCode;
}