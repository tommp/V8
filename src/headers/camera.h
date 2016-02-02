#ifndef CAMERA_H
#define CAMERA_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "display.h"
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
class Camera {
private:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 world_up;
	
	glm::vec3 camera_direction;
	glm::vec3 camera_up;
	glm::vec3 right;

	glm::vec3 offset;
	glm::mat4 view;
	bool ortographic;
public:
	Camera();
	const glm::mat4* get_view_matrix() {return &view;};
	void update_view_matrix();
	void upload_view_matrix(GLuint matrix_uniform_buffer);
	void set_target(const glm::vec3& targ) {target = targ;};
	void set_relative_position(const glm::vec3& pos);
	void set_world_up(const glm::vec3& w_up) {world_up = w_up;};

	GLfloat get_x(){return position.x;};
	GLfloat get_y(){return position.y;};
	GLfloat get_z(){return position.z;};
	void set_x(GLfloat newx){position[0] = newx;};
	void set_y(GLfloat newy){position[1] = newy;};
	void set_z(GLfloat newz){position[2] = newz;};

};
/*=============================================*/

#endif