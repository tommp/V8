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
		glm::vec3 collision_elipsoid;
		glm::vec3 size;
		glm::vec3 scale;
		glm::vec3 direction;
	public:
		const glm::vec3* get_position()const {return &position;};
		const glm::vec3* get_size()const {return &size;};

		float get_x()const{return position[0];};
		float get_y()const{return position[1];};
		float get_z()const{return position[2];};

		float get_width()const{return size[0];};
		float get_height()const{return size[1];};
		float get_depth()const{return size[2];};

		void set_x(float new_x){position[0] = new_x;};
		void set_y(float new_y){position[1] = new_y;};
		void set_z(float new_z){position[2] = new_z;};

		void set_width(float new_width){size[0] = new_width;};
		void set_height(float new_height){size[1] = new_height;};
		void set_depth(float new_depth){size[2] = new_depth;};
};

typedef std::shared_ptr<Actor> Actor_ptr;
/*=============================================*/

#endif