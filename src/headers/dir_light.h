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
		/* Shadow info */
		GLboolean render_shadows;
		GLfloat stepsize;
		GLfloat shadow_slack;
		GLfloat loop_offset;
		GLfloat probe_length;
		
		bool bind_lambda_expression()const;
	public:
		Directional_light();
		Directional_light(const glm::vec3& dir, 
						const glm::vec3& color, 
						const glm::vec3& color_components);
};
typedef std::shared_ptr<Directional_light> Directional_light_ptr;
/*=============================================*/

#endif