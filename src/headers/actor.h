#ifndef ACTOR_H
#define ACTOR_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <memory>
#include <GL/glew.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Actor{
	protected:
		glm::vec3 position;
		GLfloat width;
		GLfloat height;
		GLfloat depth;
	public:
		const glm::vec3* get_position()const {return &position;};

		float get_x()const{return position[0];};
		float get_y()const{return position[1];};
		float get_z()const{return position[2];};

		float get_width()const{return width;};
		float get_height()const{return height;};
		float get_depth()const{return depth;};

		void set_x(float new_x){position[0] = new_x;};
		void set_y(float new_y){position[1] = new_y;};
		void set_z(float new_z){position[2] = new_z;};

		void set_width(float new_width){width = new_width;};
		void set_height(float new_height){height = new_height;};
		void set_depth(float new_depth){depth = new_depth;};
};

typedef std::shared_ptr<Actor> Actor_ptr;
/*=============================================*/

#endif