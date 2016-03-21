#ifndef CAMERA_H
#define CAMERA_H


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
#include <GL/glew.h>
#include <string>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Actor;

class Camera {
	private:
		glm::vec3 init_position;
		glm::vec3 position;
		glm::vec3 target;
		glm::vec3 world_up;
		
		glm::vec3 camera_direction;
		glm::vec3 camera_up;
		glm::vec3 right;

		glm::vec3 offset;
		glm::mat4 view;

		GLboolean ortographic;
	public:
		Camera();
		bool focus_target(const glm::vec3& focus_target);
		void set_target(const glm::vec3& targ) {target = targ;};
		void set_position(const glm::vec3& pos){position = pos;};
		void set_world_up(const glm::vec3& w_up) {world_up = w_up;};
		bool center_camera(const glm::vec3& poition);
		const glm::vec3& get_position_refrence()const{return position;};
		const glm::vec3& get_up_dir_refrence()const{return camera_up;};
		const glm::vec3& get_target_refrence()const{return target;};

		GLfloat get_x(){return position.x;};
		GLfloat get_y(){return position.y;};
		GLfloat get_z(){return position.z;};
		GLfloat get_ox(){return offset.x;};
		GLfloat get_oy(){return offset.y;};
		GLfloat get_oz(){return offset.z;};
		GLfloat get_tx(){return target.x;};
		GLfloat get_ty(){return target.y;};
		GLfloat get_tz(){return target.z;};
		void set_x(GLfloat newx){position[0] = newx + init_position[0] + offset[0];};
		void set_y(GLfloat newy){position[1] = newy + init_position[1] + offset[1];};
		void set_z(GLfloat newz){position[2] = newz + init_position[2] + offset[2];};
		void set_tx(GLfloat newx){target[0] = newx + offset[0];};
		void set_ty(GLfloat newy){target[1] = newy + offset[1];};
		void set_tz(GLfloat newz){target[2] = newz + offset[2];};

};

typedef std::shared_ptr<Camera> Camera_ptr;
/*=============================================*/

#endif