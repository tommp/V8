#ifndef RENDERER_H
#define RENDERER_H


/*Included headers*/
/*---------------------------------------------*/
#include "paths.h"
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "camera.h"
#include "enum_light_type.h"
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
#include <list>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

const unsigned int OPENGL_MAJOR_VERSION =	3;
const unsigned int OPENGL_MINOR_VERSION =	3;
const unsigned int SCREEN_HEIGHT =			640;
const unsigned int SCREEN_WIDTH	=			1280;

const glm::vec4 CLEARCOLOR = 				{0.0, 0.0, 0.0, 1.0};

class Material;
class Character;
class Camera;
class Resource_manager;

typedef std::shared_ptr<Material> Material_ptr;
typedef std::shared_ptr<Character> Character_ptr;

class Renderer{
	private:
		SDL_Window* window;

		SDL_GLContext gl_context;

	    bool use_vsync;
	    bool use_fullscreen;
	    bool mouse_visible;
	    bool ortographic;

	    glm::vec2 window_size;

	    glm::mat4 projection;
	    glm::mat4 view;

		GLuint g_buffer;
		GLuint g_position;
		GLuint g_normal;
		GLuint g_albedo_spec;
		GLuint g_rbo_depth;

		std::unordered_map<std::string, GLuint> uniform_buffers;

		Shader_ptr dir_light_shader;
		Shader_ptr point_light_shader;
		Shader_ptr spot_light_shader;
		Shader_ptr geometry_shader;
	public:
		Renderer();
		Renderer(Resource_manager& resource_manager);
		bool init_window();
		bool init_openGL();
		bool init_settings();
		bool init_uniform_buffers();
		bool init_framebuffer();
		bool init_shaders(Resource_manager& resource_manager);

		bool use_g_buffer()const;
		bool use_default_buffer()const;
		bool use_light_shader(Light_type light_type)const;
		bool bind_g_data(Light_type light_type)const;
		bool unbind_g_data()const;
		bool set_clear_color_black();

		glm::mat4 get_projection_matrix()const{return projection;};
		glm::mat4 get_view_matrix()const{return view;};

		GLuint get_uniform_buffer(const std::string& name)const;
		GLuint get_light_shader_program(Light_type light_type)const;
		GLuint get_window_width()const{return window_size.x;};
		GLuint get_window_height()const{return window_size.y;};
		Shader_ptr get_light_shader(Light_type light_type)const;

		void setup_geometry_rendering(const Camera_ptr& camera);
		bool render_geometry(GLuint VAO, 
							GLuint num_vertices,
							const Material_ptr& material, 
							const glm::vec3& position, 
							const glm::vec3& size, 
							const glm::vec3& direction)const;
		bool render_geometry(std::vector<const std::list<Character_ptr>*> targets, 
										const Camera_ptr& camera);
		void detach_geometry_rendering()const;

		void setup_light_rendering(Light_type light_type, const glm::vec3& position)const;
		bool render_light()const;
		void detach_light_rendering()const;

		void upload_view_position(GLuint shader_program, const glm::vec3& position)const;

		void upload_view_matrix()const;
		void update_view_matrix(const glm::vec3& position, 
							const glm::vec3& target, 
							const glm::vec3& camera_up);

		void update_projection_matrix();
		void upload_projection_matrix()const;

		bool upload_light_data()const;

		bool save_settings();
		bool load_settings();
		bool enable_fullscreen();
		bool enable_vsync();
		bool disable_vsync();

		void toggle_mouse()const;
		void clear()const;
		void present()const;
};
/*=============================================*/

#endif