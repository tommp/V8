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
		GLfloat cutoff;
		GLfloat outer_cutoff;
	  
		GLfloat radius;

		bool bind_lambda_expression()const;
		bool calculate_light_uniforms();
	public:
		Spot_light();
		Spot_light(GLfloat radius, 
				const glm::vec3& pos,
				const glm::vec3& dir,
				const glm::vec3& color, 
				const glm::vec3& color_components,
				const GLfloat cutoff,
				const GLfloat outer_cutoff);
};
typedef std::shared_ptr<Spot_light> Spot_light_ptr;
/*=============================================*/

#endif