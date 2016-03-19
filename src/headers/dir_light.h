#ifndef DIR_LIGHT_H
#define DIR_LIGHT_H

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
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Light;

class Directional_light : public Light {
	public:
		Directional_light();
		bool init_light_quad();
		bool render_light_quad()const;
		bool render_light(const Shader_ptr& shader)const;
};
typedef std::shared_ptr<Directional_light> Directional_light_ptr;
/*=============================================*/

#endif