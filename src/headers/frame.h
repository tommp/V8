#ifndef FRAME_H
#define FRAME_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "mesh.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Frame{
private:
	Mesh_ptr mesh;
	GLuint frame_duration;
public:
	GLuint get_duration(){return frame_duration;};
	void render(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, GLfloat rotate);
};
/*=============================================*/

#endif