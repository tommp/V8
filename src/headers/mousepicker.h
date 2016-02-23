#ifndef MOUSEPICKER_H
#define MOUSEPICKER_H


/*Included headers*/
/*---------------------------------------------*/
#include "btBulletDynamicsCommon.h"
#include "glm.h"
#include "errorlogger.h"
#include "renderer.h"
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
class Mousepicker{
private:
	glm::vec4 ray_start;
	glm::vec4 ray_end;
public:
	void calculate_ray(GLfloat length, const Renderer& renderer);
	void check_for_intersection(const btDiscreteDynamicsWorld* physics_world);
};
/*=============================================*/

#endif