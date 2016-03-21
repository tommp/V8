#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "enum_light_type.h"
#include "errorlogger.h"
#include "utility.h"
#include "base_light.h"
#include "shader.h"
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
class Light;

class Point_light : public Base_light {
	private:
		GLfloat linear;
		GLfloat quadratic;
		bool bind_lambda_expression()const;
	public:
		Point_light();
		bool calculate_light_uniforms(GLfloat linear, GLfloat quadratic, GLfloat intensity);
	
};
typedef std::shared_ptr<Point_light> Point_light_ptr;
/*=============================================*/

#endif