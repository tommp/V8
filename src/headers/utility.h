#ifndef UTILITY_H
#define UTILITY_H

/*Included headers*/
/*---------------------------------------------*/
#include "SOIL.h"
#include "paths.h"
#include "errorlogger.h"
#include "vertex.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <glob.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
#define DISABLE_VSYNC 0

const unsigned int TILESIZE = 				32;
const unsigned int SCREEN_FPS = 			60;
const int SCREEN_TICKS_PER_FRAME =			1000 / SCREEN_FPS;

/* Waits for user input and quits when detected */
void wait_for_event();

std::vector<std::string> glob(const std::string& path);

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

std::vector<std::string> split(const std::string &s, char delim);

bool convert_all_models();

bool convert_model_file();

bool store_binary_model();

bool store_binary_mesh();

bool convert_all_images();

bool convert_image_file(const std::string& source_path, const std::string& target_path);

bool store_binary_texture(const std::string& path, 
							unsigned char* image, 
							GLuint width, 
							GLuint height, 
							GLuint channels, 
							GLint format);

/* Print a verbose error message */
const char* gl_error_string(GLenum err);

/* Check for openGL errors and print them to file if detected */
int check_ogl_error();

void print_framebuffer_error_in_fucking_english();
/*=============================================*/

#endif
