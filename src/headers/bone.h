#ifndef BONE_H
#define BONE_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <string>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

typedef struct Bone{
	std::string name;
	glm::mat4 offset;
}Bone;

/*=============================================*/

#endif