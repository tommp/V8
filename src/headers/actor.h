#ifndef ACTOR_H
#define ACTOR_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <memory>
#include <GL/glew.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Actor{
	protected:
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 direction;
		glm::vec3 init_direction;
	public:
		Actor();

		float get_x()const{return position[0];};
		float get_y()const{return position[1];};
		float get_z()const{return position[2];};

		float get_width()const{return scale[0];};
		float get_height()const{return scale[1];};
		float get_depth()const{return scale[2];};

		void set_x(float new_x){position[0] = new_x;};
		void set_y(float new_y){position[1] = new_y;};
		void set_z(float new_z){position[2] = new_z;};

		void set_width(float new_width){scale[0] = new_width;};
		void set_height(float new_height){scale[1] = new_height;};
		void set_depth(float new_depth){scale[2] = new_depth;};
};

typedef std::shared_ptr<Actor> Actor_ptr;
/*=============================================*/

#endif