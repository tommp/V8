#ifndef CAMERA_H
#define CAMERA_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "actor.h"
#include "display.h"
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
class Character;
class Actor;

typedef std::shared_ptr<Character> Character_ptr;
typedef std::shared_ptr<Actor> Actor_ptr;

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

		bool ortographic;
	public:
		Camera();
		const glm::mat4* get_view_matrix() {return &view;};
		void update_view_matrix();
		void upload_view_matrix(GLuint matrix_uniform_buffer)const;
		void focus_target(const glm::vec3& focus_target);
		void set_target(const glm::vec3& targ) {target = targ;};
		void set_position(const glm::vec3& pos){position = pos;};
		void set_world_up(const glm::vec3& w_up) {world_up = w_up;};
		void center_camera(const Actor_ptr& target, GLuint bound_width, GLuint bound_height);
		const glm::vec3* get_position_pointer()const{return &position;};

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
/*=============================================*/

#endif