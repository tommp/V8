#ifndef LIGHT_H
#define LIGHT_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "actor.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Light : public Actor {
private:
	glm::vec3 color;
public:

};
/*=============================================*/

#endif