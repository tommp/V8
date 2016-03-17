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

class Point_light : public Light {
	private:
		GLfloat linear;
		GLfloat quadratic;
		glm::mat4 quad_model_matrix;
	public:
		Point_light();
		bool init_light_quad();
		bool render_light_quad()const;
		bool render_light(const Shader_ptr& shader)const;
	
};
typedef std::shared_ptr<Point_light> Point_light_ptr;
/*=============================================*/

#endif