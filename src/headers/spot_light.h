#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

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

class Spot_light : public Base_light{
	private:
		GLfloat cut_off;
		GLfloat outer_cut_off;
	  
		GLfloat radius;

		bool bind_lambda_expression()const;
		bool calculate_light_uniforms();
	public:
		Spot_light();
};
typedef std::shared_ptr<Spot_light> Spot_light_ptr;
/*=============================================*/

#endif