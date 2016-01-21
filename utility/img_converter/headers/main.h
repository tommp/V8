#ifndef MAIN_H
#define MAIN_H


/*Included headers*/
/*---------------------------------------------*/
#include "./image.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <glob.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
#define IMAGES_DIR 		"./images/*"
#define REGISTERFILE 	"../../data/register.blue"
#define CONTENTFILE 	"../../data/content.blue"
#define ID_FILE			"../../src/headers/image_map.h"

std::vector<std::string> glob(const std::string& pat);
/*=============================================*/

#endif