#ifndef RENDERER_H
#define RENDERER_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "camera.h"
#include "display.h"
#include "character.h"
#include "material.h"
#include "resource_manager.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Material;
class Character;
class Camera;
class Resource_manager;
class Display;

typedef std::shared_ptr<Material> Material_ptr;
typedef std::shared_ptr<Character> Character_ptr;

class Renderer{
	private:
		Display* display;

		Camera camera;

		GLuint g_buffer;

		GLuint g_position;
		GLuint g_normal;
		GLuint g_albedo_spec;
		GLuint g_rbo_depth;

		std::unordered_map<std::string, GLuint> uniform_buffers;

		GLuint quad_VAO;
		Shader_ptr light_shader;
		Shader_ptr geometry_shader;
	public:
		Renderer();
		Renderer(Display& display, Resource_manager& resource_manager);

		GLuint get_uniform_buffer(const std::string& name)const;
		bool use_g_buffer()const;
		bool use_default_buffer()const;
		bool bind_g_data()const;
		bool unbind_g_data()const;
		bool set_clear_color_black();

		GLuint get_light_shader()const{return light_shader->get_program();};

		bool init_matrix_uniform_buffer();
		bool init_light_quad();
		bool init_framebuffer();
		bool init_shaders(Resource_manager& resource_manager);

		void setup_geometry_rendering();
		bool render_geometry(GLuint VAO, 
							GLuint num_vertices,
							const Material_ptr& material, 
							const glm::vec3& position, 
							const glm::vec3& size, 
							const glm::vec3& direction)const;
		void detach_geometry_rendering()const;


		void clear_display()const{display->clear();};
		void present_display()const{display->present();};

		void setup_light_rendering()const;
		bool render_light()const;
		bool render_light_quad()const;
		void detach_light_rendering()const;

		void center_camera(const Actor_ptr& target, GLuint bound_width, GLuint bound_height);
		void upload_view_matrix(GLuint uniform_matrix_buffer)const{camera.upload_view_matrix(uniform_matrix_buffer);};
		void update_view_matrix(){camera.update_view_matrix();};
};
/*=============================================*/

#endif