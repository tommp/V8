#ifndef CHANNEL_H
#define CHANNEL_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <GL/glew.h>
#include <vector>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
typedef struct Channel{
	std::vector<std::pair<GLdouble, glm::vec3>> positions;
	std::vector<std::pair<GLdouble, glm::vec3>> scalings;
	std::vector<std::pair<GLdouble, glm::fquat>> rotations;
}Channel;
/*=============================================*/

#endif

