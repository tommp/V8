#ifndef BASE_LIGHT_H
#define BASE_LIGHT_H

/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "enum_light_type.h"
#include "object.h"
#include "errorlogger.h"
#include "utility.h"
#include "base_geometry.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <GL/glew.h>
#include <string>
#include <unistd.h>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Object;
class Renderer;
class Shader;

typedef std::shared_ptr<Shader> Shader_ptr;

class Light : public Object {
	protected:
		GLuint clip_VAO;
		Base_geometry_ptr base_geometry;
		Light_type type;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	public:
		Light();
		virtual bool render_light(const Shader_ptr& shader)const = 0;
		Light_type get_type(){return type;};
		bool update_position(float timedelta);
		bool update_context();
		bool touch_object(Object& object);
		bool add_bases_to_context();
};
typedef std::shared_ptr<Light> Light_ptr;
/*=============================================*/

#endif