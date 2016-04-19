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
		GLfloat radius;

		/* Shadow info */
		GLboolean render_shadows;

		bool bind_lambda_expression()const;
	public:
		Point_light();
		Point_light(GLfloat radius, 
					const glm::vec3& pos, 
					const glm::vec3& color, 
					const glm::vec3& color_components,
					GLboolean render_shadows = false);
		bool calculate_light_uniforms();
};
typedef std::shared_ptr<Point_light> Point_light_ptr;
/*=============================================*/

#endif