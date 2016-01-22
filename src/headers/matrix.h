#ifndef MATRIX_H
#define MATRIX_H


/*Included headers*/
/*---------------------------------------------*/
#include "./errorlogger.h"
#include "./utility.h"
#include "./vector.h"
#include "./quaternion.h"
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
class Matrix{
	private:
		GLfloat matrix[16];
	public:
		Matrix();
		void set_translator(const Vec3& trans);
		void set_rotator(const Quaternion& rot);

		GLfloat operator()(int x, int y) const;
};
/*=============================================*/

#endif