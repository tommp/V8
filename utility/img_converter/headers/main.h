#ifndef MAIN_H
#define MAIN_H


/*Included headers*/
/*---------------------------------------------*/
#include "./SOIL.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <glob.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
#define IMAGES_DIR 		"./images/*"
#define CONTENTFILE 	"../../data/content.blue"
#define ID_FILE			"../../src/headers/image_map.h"

std::vector<std::string> glob(const std::string& pat);
void store_as_binary(GLuint* id, unsigned char* image, GLint width, GLint height, GLint format, char* datafile);
/*=============================================*/

#endif