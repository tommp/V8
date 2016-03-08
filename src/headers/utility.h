#ifndef UTILITY_H
#define UTILITY_H

/*Included headers*/
/*---------------------------------------------*/
#include "SOIL.h"
#include "paths.h"
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
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
#define DISABLE_VSYNC 0

const unsigned int TILESIZE = 				32;
const unsigned int SCREEN_FPS = 			60;
const int SCREEN_TICKS_PER_FRAME =			1000 / SCREEN_FPS;
const GLuint TRUE_BOOL = 					1;
const GLuint FALSE_BOOL = 					0;

inline GLboolean file_exists(const std::string& name);

/* Waits for user input and quits when detected */
void wait_for_event();

GLboolean write_string_to_binary_file(std::ofstream& fstream, const std::string& string);

std::string read_string_from_binary_file(std::ifstream& fstream);

GLboolean write_vector_to_binary_file(std::ofstream& contentf, const aiVectorKey& vector);

GLboolean write_quaternion_to_binary_file(std::ofstream& contentf, const aiQuatKey& quaternion);

std::string build_anim_set_name(const std::vector<std::string>& nodenames);

void store_ai_node_tree(std::ofstream& contentf, aiNode* node);

std::vector<std::string> glob(const std::string& path);

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

std::vector<std::string> split(const std::string &s, char delim);

GLboolean convert_all_models();

GLboolean convert_model_file(const std::string& source_path, const std::string& target_path);

void process_node(aiNode* node, const aiScene* scene, 
					std::vector<std::string>& mesh_names);

std::string process_mesh(aiMesh* mesh, const aiScene* scene);

void store_binary_mesh(const std::vector<Vertex>& vertices, 
						const std::vector<GLuint>& indices, 
						const std::string& material_name,
						std::string& meshname);

void store_binary_mesh(const aiScene* scene,
						const std::vector<Vertex>& vertices, 
						const std::vector<GLuint>& indices, 
						const std::string& material_name,
						std::string& meshname,
						const std::unordered_map<std::string, GLuint>& bone_map,
						const std::vector<glm::mat4>& bone_info);

GLboolean store_binary_material(const aiScene* scene, aiMesh* mesh, std::string& material_name);

GLboolean store_binary_animation_set(const aiScene* scene, const std::string& modelname);

void load_mesh_bones(const aiMesh* mesh, 
						std::unordered_map<std::string, GLuint>& bone_map,
						std::vector<Vertex>& vertices,
						std::vector<glm::mat4>& bone_info);

std::vector<std::string> load_material_textures(aiMaterial* mat, 
												aiTextureType type, 
												const std::string& typeName);

std::string load_material_texture(aiMaterial* mat, 
									aiTextureType type, 
									const std::string& typeName);

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

void print_framebuffer_error_in_fucking_english();
/*=============================================*/

#endif
