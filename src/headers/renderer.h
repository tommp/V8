#ifndef RENDERER_H
#define RENDERER_H


/*Included headers*/
/*---------------------------------------------*/
#include "paths.h"
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "base_geometry.h"
#include "camera.h"
#include "enum_light_type.h"
#include "light.h"
#include "object.h"
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
#include <forward_list>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

const unsigned int OPENGL_MAJOR_VERSION =	3;
const unsigned int OPENGL_MINOR_VERSION =	3;

const glm::vec4 CLEARCOLOR = 				{0.0, 0.0, 0.0, 1.0};

class Material;
class Object;
class Camera;
class Resource_manager;
class Light;

typedef std::shared_ptr<Light> Light_ptr;
typedef std::shared_ptr<Material> Material_ptr;
typedef std::shared_ptr<Object> Object_ptr;

class Renderer{
	private:
		SDL_Window* window;

		SDL_GLContext gl_context;

		Base_geometry base_geom_line;
		Base_geometry base_geom_box;
		Base_geometry base_geom_sphere;

	    GLboolean use_vsync;
	    GLboolean use_fullscreen;
	    GLboolean mouse_visible;
	    GLboolean ortographic;

	    glm::vec2 window_size;

	    glm::mat4 projection;
	    glm::mat4 view;

		GLuint g_buffer;
		GLuint g_position;
		GLuint g_normal;
		GLuint g_albedo_spec;
		GLuint g_rbo_depth;

		GLuint uniform_buffer_matrices;
		GLuint uniform_buffer_light_data;

		std::unordered_map<std::string, GLuint> uniform_buffers;

		std::list<Rendering_context_weak> animated_targets;
		std::list<Rendering_context_weak> static_targets;

		Shader_ptr dir_light_shader;
		Shader_ptr point_light_shader;
		Shader_ptr spot_light_shader;
		Shader_ptr geometry_shader;
		Shader_ptr animated_geometry_shader;
	public:
		Renderer();
		Renderer(Resource_manager& resource_manager);
		~Renderer();
		bool init_window();
		bool init_openGL();
		bool init_settings();
		bool init_uniform_buffers();
		bool init_framebuffer();
		bool init_shaders(Resource_manager& resource_manager);
		bool init_base_geometry();

		bool delete_g_buffer();

		bool use_g_buffer()const;
		bool use_default_buffer()const;
		bool use_light_shader(Light_type light_type)const;
		bool bind_g_data(Light_type light_type)const;
		bool unbind_g_data()const;
		bool set_clear_color_black();
		bool add_context(const Rendering_context_ptr& context);

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
							const glm::vec3& direction,
							GLenum mode)const;

		bool render_geometry(std::vector<const std::list<Object_ptr>*> targets, 
										const Camera_ptr& camera);

		bool render_dir_lights(const std::forward_list<Light_ptr>& dir_lights, 
								const glm::vec3& position)const;
		bool render_point_lights(const std::forward_list<Light_ptr>& point_lights, 
								const glm::vec3& position)const;
		bool render_spot_lights(const std::forward_list<Light_ptr>& spot_lights, 
								const glm::vec3& position)const;


		bool render_line(const glm::vec3& start, 
							const glm::vec3& end, 
							const glm::vec3& color);

		void detach_geometry_rendering()const;

		void setup_light_rendering(Light_type light_type, const glm::vec3& position)const;
		bool render_light()const;
		void detach_light_rendering()const;

		void upload_view_position(Shader& shader, const glm::vec3& position)const;

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
		bool set_screen_size(GLuint width, GLuint height);
		bool update_screen_size();
		bool enable_vsync();
		bool disable_vsync();

		void toggle_mouse()const;
		void clear()const;
		void present()const;
};

typedef std::shared_ptr<Renderer> Renderer_ptr;
/*=============================================*/

#endif