#ifndef UTILITY_H
#define UTILITY_H

/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
#include "vertex.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <unordered_map>
#include <vector>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
#define DISABLE_VSYNC 0
#define IMAGE_DATA_PATH "../data/content.blue"
#define MESH_DATA_PATH "../data/content.boogie"

const unsigned int TILESIZE = 				32;
const unsigned int SCREEN_FPS = 			60;
const int SCREEN_TICKS_PER_FRAME =			1000 / SCREEN_FPS;

/* Waits for user input and quits when detected */
void wait_for_event();

/* Loads a binary image */
bool load_binary_image(const std::string& name, unsigned char* image, GLuint* width, GLuint* height, GLint* format);

/* Loads a binary mesh */
bool load_binary_mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

/* Print a verbose error message */
const char* gl_error_string(GLenum err);

/* Check for openGL errors and print them to file if detected */
int check_ogl_error();
/*=============================================*/

#endif
