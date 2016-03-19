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
#include "base_geometry.h"
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

class Spot_light : public Light{
	private:
		GLfloat cut_off;
		GLfloat outer_cut_off;
	  
		GLfloat linear;
		GLfloat quadratic;
	public:
		Spot_light();
		bool init_light_quad();
		bool render_light_quad()const;
		bool render_light(const Shader_ptr& shader)const;  
};
typedef std::shared_ptr<Spot_light> Spot_light_ptr;
/*=============================================*/

#endif