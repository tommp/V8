#ifndef VECTOR_H
#define VECTOR_H


/*Included headers*/
/*---------------------------------------------*/
#include "./errorlogger.h"
#include "./utility.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Vec3{
	private:
		GLfloat vector[3];
	public:
		Vec3();
		GLfloat operator()(int x) const;
		GLfloat& operator[](int x);
};
/*=============================================*/

#endif