#ifndef BASE_GEOMETRY_H
#define BASE_GEOMETRY_H


/*Included headers*/
/*---------------------------------------------*/
#include "enum_geom_type.h"
#include "glm.h"
#include "utility.h"
#include "errorlogger.h"
#include "rendering_context.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <GL/glew.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Base_geometry{
	private:
		Base_render_context_ptr rendering_context;

		GLuint VBO;
		glm::vec4 color;
	public:
		Base_geometry();
		Base_geometry(Geom_type type, const glm::vec4& color);
		~Base_geometry();
		GLuint get_vertex_array()const{return VAO;};
		bool gen_arrays(GLfloat* vertices);
};

typedef std::shared_ptr<Base_geometry> Base_geometry_ptr;
/*=============================================*/

#endif