#ifndef BONE_H
#define BONE_H

/* TODO::FOR LATER USE */
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
	glm::vec4 weights;
	glm::i16vec4 bone_ids;
}Bone;

/*=============================================*/

#endif