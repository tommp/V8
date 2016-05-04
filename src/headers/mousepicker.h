#ifndef MOUSEPICKER_H
#define MOUSEPICKER_H


/*Included headers*/
/*---------------------------------------------*/
#include "btBulletDynamicsCommon.h"
#include "glm.h"
#include "errorlogger.h"
#include "renderer.h"

#include "prop.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>
#include <unistd.h>
#include <memory>
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

typedef std::shared_ptr<Mousepicker> Mousepicker_ptr;
/*=============================================*/

#endif