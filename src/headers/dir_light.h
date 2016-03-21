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

class Directional_light : public Base_light {
	private:
		glm::vec3 light_direction;
	public:
		Directional_light();
		bool bind_lambda_expression()const;
		bool init_light_quad();
};
typedef std::shared_ptr<Directional_light> Directional_light_ptr;
/*=============================================*/

#endif