#ifndef LINE_DATA_H
#define LINE_DATA_H

/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <memory>
#include <unordered_map>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
struct Line_data{
	glm::mat4 model_matrix;
	glm::vec3 color;
};
/*=============================================*/

#endif