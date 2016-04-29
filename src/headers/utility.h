#ifndef UTILITY_H
#define UTILITY_H

/*Included headers*/
/*---------------------------------------------*/
#include "SOIL.h"
#include "DEFINES.h"
#include "errorlogger.h"
#include "vertex.h"
#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <GL/glew.h>
#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <glob.h>
#include <cstdio>
#include <unistd.h>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

namespace Utility_vars{
	extern std::string folder_path;
}

namespace Utility_consts{
	const GLuint SCREEN_FPS = 				60;
	const GLuint SCREEN_TICKS_PER_FRAME =	1000 / SCREEN_FPS;
	const GLuint TRUE_BOOL = 				1;
	const GLuint FALSE_BOOL = 				0;
	
	/* Light uniform specs. */
	const GLuint SIZEOF_DIR_LIGHT = 	3 * sizeof(glm::vec4);
	const GLuint SIZEOF_POINT_LIGHT = 	sizeof(glm::mat4) + 3 * sizeof(glm::vec4);
	const GLuint SIZEOF_SPOT_LIGHT = 	sizeof(glm::mat4) + 5 * sizeof(glm::vec4);

	const GLuint MAX_DIR_LIGHTS = 		10;
	const GLuint MAX_POINT_LIGHTS = 	100;
	const GLuint MAX_SPOT_LIGHTS = 		100;
}



inline GLboolean file_exists(const std::string& name);

void wait_for_event();

GLboolean write_string_to_binary_file(std::ofstream& fstream, const std::string& string);

bool read_string_from_binary_file(std::ifstream& fstream, std::string& data);

GLboolean write_vector_to_binary_file(std::ofstream& contentf, const aiVectorKey& vector);

GLboolean write_quaternion_to_binary_file(std::ofstream& contentf, const aiQuatKey& quaternion);

GLboolean read_vector_from_binary_file(std::ifstream& contentf, 
										std::pair<GLdouble, glm::vec3>& key_vector);

GLboolean read_quaternion_from_binary_file(std::ifstream& contentf, 
										std::pair<GLdouble, glm::fquat>& key_quaternion);

std::string build_anim_set_name(const std::vector<std::string>& nodenames);

void store_ai_node_tree(std::ofstream& contentf, const aiNode* node, GLboolean root, 
						const std::vector<std::string>& bone_names, 
						const std::unordered_map<std::string, GLuint>& bone_id_map);

std::vector<std::string> glob(const std::string& path);

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

std::vector<std::string> split(const std::string &s, char delim);

GLboolean convert_all_models();

GLboolean convert_model_file(const std::string& source_path, const std::string& target_path);

void process_node(const aiNode* node, 
					const aiScene* scene, 
					std::vector<std::string>& mesh_names,
					std::unordered_map<std::string, GLuint>& bone_id_map,
					const std::string& modelname);

std::string process_mesh(const aiMesh* mesh, const aiScene* scene, 
						std::unordered_map<std::string, GLuint>& bone_id_map,
						const std::string& modelname);

void store_binary_mesh(const std::vector<Vertex>& vertices, 
						const std::vector<GLuint>& indices, 
						const std::string& material_name,
						std::string& meshname,
						const std::string& modelname);

void store_binary_mesh(const aiScene* scene,
						const std::vector<Vertex>& vertices, 
						const std::vector<GLuint>& indices, 
						const std::string& material_name,
						std::string& meshname,
						const std::string& modelname,
						const std::unordered_map<GLuint, GLuint>& bone_map,
						const std::vector<glm::mat4>& bone_info);

GLboolean store_binary_material(const aiScene* scene, const aiMesh* mesh, std::string& material_name);

GLboolean store_binary_animation_set(const aiScene* scene, const std::string& modelname, 
								const std::unordered_map<std::string, GLuint>& bone_id_map);

void load_mesh_bones(const aiMesh* mesh, 
						std::unordered_map<GLuint, GLuint>& bone_map,
						std::unordered_map<std::string, GLuint>& bone_id_map,
						std::vector<Vertex>& vertices,
						std::vector<glm::mat4>& bone_info);

std::vector<std::string> load_material_textures(const aiMaterial* mat, 
												aiTextureType type);

std::string load_material_texture(const aiMaterial* mat, 
									aiTextureType type);

GLboolean convert_all_images();

GLboolean convert_image_file(const std::string& source_path, const std::string& target_path);

GLboolean store_binary_texture(const std::string& path, 
							unsigned char* image, 
							GLuint width, 
							GLuint height, 
							GLuint channels, 
							GLint format);

/* Print a verbose error message */
const char* gl_error_string(GLenum err);

/* Check for openGL errors and print them to file if detected */
GLint check_ogl_error();

void discard_ogl_errors();

void print_framebuffer_error_in_english();

bool init_utility_vars();

bool set_folder_path();
/*=============================================*/

#endif
