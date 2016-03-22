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
		bool bind_lambda_expression()const;
		bool init_light_quad();
	public:
		Directional_light();
};
typedef std::shared_ptr<Directional_light> Directional_light_ptr;
/*=============================================*/

#endif