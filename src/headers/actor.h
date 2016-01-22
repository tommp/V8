#ifndef ACTOR_H
#define ACTOR_H


/*Included headers*/
/*---------------------------------------------*/
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
		GLfloat x;
		GLfloat y;
		GLfloat z;
		GLfloat width;
		GLfloat height;
		GLfloat depth;
	public:
		float get_x()const{return x;};
		float get_y()const{return y;};
		float get_z()const{return z;};

		float get_width()const{return width;};
		float get_height()const{return height;};
		float get_depth()const{return depth;};

		void set_x(float new_x){x = new_x;};
		void set_y(float new_y){y = new_y;};
		void set_z(float new_z){z = new_z;};

		void set_width(float new_width){width = new_width;};
		void set_height(float new_height){height = new_height;};
		void set_depth(float new_depth){depth = new_depth;};
};

typedef std::shared_ptr<Actor> Actor_ptr;
/*=============================================*/

#endif