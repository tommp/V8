#ifndef CAMERA_H
#define CAMERA_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
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
	
	glm::vec3 direction;
	glm::vec3 up;
	glm::vec3 right;

	glm::mat4 view;
public:
	Camera();
	Camera(const glm::vec3& pos, const glm::vec3& targ, const glm::vec3& w_up);
	const glm::mat4* get_view_matrix() {return &view;};
	void update_view_matrix();
	void set_taget(const glm::vec3& targ) {target = targ;};
	void set_position(const glm::vec3* pos){position = *pos;};
	void set_world_up(const glm::vec3& w_up) {world_up = w_up;};

	glm::vec3* get_position() {return &position;};
	
};
/*=============================================*/

#endif