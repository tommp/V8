#ifndef CHANNEL_H
#define CHANNEL_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <vector>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
typedef struct Channel{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> scalings;
	std::vector<glm::fquat> rotations;
}Channel;
/*=============================================*/

#endif

