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
#include "material.h"
#include "resource_manager.h"
#if ENABLE_BULLET_DEBUG
#include "line_data.h"
#endif
#include "mesh.h"
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

namespace Renderer_consts{
	const GLuint SHADOW_LAYERS = 		1;
	const GLuint BATCH_SIZE = 			100;
	const GLuint OPENGL_MAJOR_VERSION =	3;
	const GLuint OPENGL_MINOR_VERSION =	2;
	const glm::vec4 CLEARCOLOR = 		{0.0, 0.0, 0.0, 1.0};
}

class Mesh;
class Resource_manager;

typedef std::shared_ptr<Mesh> Mesh_ptr;

class Renderer{
	private:
		SDL_Window* window;
		SDL_GLContext gl_context;

	    GLboolean use_vsync;
	    GLboolean use_AA;
	    GLboolean use_SSAO;
	    GLboolean use_bloom;
	    GLboolean use_shadows;
	    GLboolean use_fullscreen;
	    GLboolean use_mouse;
	    GLboolean ortographic;

	    GLboolean clear_shadow_layers[Renderer_consts::SHADOW_LAYERS];

	    GLuint last_blurred;

	    GLfloat near_plane;
	    GLfloat far_plane;

	    glm::vec2 window_size;
	    glm::vec2 resolution;

	    glm::mat4 projection;
	    glm::mat4 view;
	    glm::mat4 unrotated_view;

	    /* TODO::Optimize lots here, wasting a ton of res. */
		GLuint g_buffer;
		GLuint AA_fbo;
		GLuint SSAO_fbo;
		GLuint bb_fbos[2];
		GLuint blurred_output;
		GLuint light_fbo;

		GLuint g_position;
		GLuint g_normal;
		GLuint g_albedo_spec;
		GLuint g_bloom;
		GLuint g_rbo_depth;
		GLuint bb_buffers[2];
		GLuint AA_buffer;
		GLuint SSAO_buffer;
		GLuint light_color_buffer;
		GLuint light_ambient_buffer;
		GLuint light_rbo_depth;

		/* LFST fbos */
		GLuint shadow_layer_fbos[Renderer_consts::SHADOW_LAYERS];
		GLuint shadow_front_cull_fbos[Renderer_consts::SHADOW_LAYERS];
		GLuint shadow_back_cull_fbos[Renderer_consts::SHADOW_LAYERS];

		/* LFST buffers */
		GLuint shadow_layers[Renderer_consts::SHADOW_LAYERS];
		GLuint shadow_front_cull_buffers[Renderer_consts::SHADOW_LAYERS];
		GLuint shadow_back_cull_buffers[Renderer_consts::SHADOW_LAYERS];
		GLuint shadow_front_cull_depth[Renderer_consts::SHADOW_LAYERS];
		GLuint shadow_back_cull_depth[Renderer_consts::SHADOW_LAYERS];

		GLuint quad_VAO;
		GLuint quad_VBO;

		GLuint cube_VAO;
		GLuint cube_VBO;
		GLuint cube_EBO;

#if ENABLE_BULLET_DEBUG
		Mesh_ptr line;
#endif

		std::unordered_map<std::string, GLuint> uniform_buffers;

		std::list<Rendering_context_weak> animated_geom;
		std::list<Rendering_context_weak> animated_colored_geom;
		std::list<Rendering_context_weak> static_geom;
		std::list<Rendering_context_weak> static_colored_geom;

		std::list<Rendering_context_light_weak> dir_lights;
		std::list<Rendering_context_light_weak> point_lights;
		std::list<Rendering_context_light_weak> spot_lights;

#if ENABLE_BULLET_DEBUG
		std::list<Line_data> lines;
#endif

		Shader_ptr dir_light_shader;
		Shader_ptr point_light_shader;
		Shader_ptr spot_light_shader;
		Shader_ptr static_geometry_shader;
		Shader_ptr static_geometry_shader_colored;
		Shader_ptr animated_geometry_shader;
		Shader_ptr animated_geometry_shader_colored;
		Shader_ptr primitive_line_shader;
		Shader_ptr vertical_blur_shader;
		Shader_ptr horizontal_blur_shader;
		Shader_ptr final_shader;
		Shader_ptr FXAA_shader;
		Shader_ptr SSAO_shader;
		Shader_ptr LFST_cull_shader;
		Shader_ptr LFST_layer_shader;

		bool init_window();
		bool init_openGL();
		bool init_settings();
		bool init_g_buffer();
		bool init_blur_buffers();
		bool init_AA_buffer();
		bool init_SSAO_buffer();
		bool init_light_buffer();
		bool init_shadow_buffers();
		bool init_framebuffers();
		bool init_uniform_buffers();
		bool init_shaders(Resource_manager& resource_manager);
		bool init_quad();
		bool init_cube();
		bool init_primitives(Resource_manager& resource_manager);
		bool init_upload_uniform_data();

		bool delete_buffers();
		bool delete_uniform_buffers();

		bool set_viewport_window()const;
		bool set_viewport_resolution()const;

		bool use_g_buffer()const;
		bool use_default_buffer()const;

		bool upload_res_data()const;
		bool upload_res_data_final_pass()const;
		bool upload_plane_data()const;
		bool upload_view_matrix()const;
		bool upload_projection_matrix()const;
		bool update_projection_matrix();
		bool upload_settings()const;

		bool setup_geometry_rendering(const Camera_ptr& camera);
		bool detach_geometry_rendering()const;
		bool render_static_geomety()const;
		bool render_static_colored_geomety()const;
		bool render_animated_geomety()const;
		bool render_animated_colored_geomety()const;
#if ENABLE_BULLET_DEBUG
		bool render_primitive_line_geometry();
#endif
		bool render_base_geometry(const Rendering_context_ptr& context, 
								const Shader_ptr& shader)const;
		bool ogl_render_geometry(const Rendering_context_ptr& context, GLuint instances)const;
		bool ogl_render_shadow_geometry(const Rendering_context_ptr& context, GLuint instances);

		bool render_quad()const;
		bool render_quad(GLuint instances)const;
		bool render_cube(GLuint instances)const;

		bool render_shadow_cull_layers();
		bool render_shadow_geometry(const Rendering_context_ptr& context);
		bool generate_shadow_layers();

		bool bind_g_data(Shader_type light_type)const;
		bool upload_view_position(Shader_type shader_type, 
								const glm::vec3& position)const;

		bool render_dir_lights();
		bool render_point_lights();
		bool render_spot_lights();
		bool render_light(const Rendering_context_light_ptr& context, 
						const Shader_ptr& shader)const;
		bool render_dir_light(const Rendering_context_light_ptr& context, 
						const Shader_ptr& shader)const;

		bool set_clear_color_black();
		GLfloat lerp(GLfloat a, GLfloat b, GLfloat f)const;
		bool blur_texture(GLuint amount, GLuint texture);
		bool update_window_size();

		bool render_geometry(const Camera_ptr& camera);
		bool render_lights(const glm::vec3& position);
		bool ppe_blend();

		bool copy_depth(GLuint source_fbo, GLuint target_fbo);

		bool apply_AA()const;
		bool apply_SSAO();
		bool apply_shadows();
		
	public:
		Renderer();
		Renderer(Resource_manager& resource_manager);
		~Renderer();
		
		bool add_context(const Rendering_context_ptr& context);
		bool add_context(const Rendering_context_light_ptr& context);

#if ENABLE_BULLET_DEBUG
		bool add_context(Line_data context);
#endif

		glm::mat4 get_projection_matrix()const{return projection;};
		glm::mat4 get_view_matrix()const{return view;};

		GLuint get_uniform_buffer(const std::string& name)const;
		GLuint get_window_width()const{return window_size.x;};
		GLuint get_window_height()const{return window_size.y;};

		bool render_line(const glm::vec3& start, 
							const glm::vec3& end, 
							const glm::vec3& color);

		void update_view_matrix(const glm::vec3& position, 
							const glm::vec3& target, 
							const glm::vec3& camera_up);

		bool save_settings();
		bool load_settings();
		bool enable_fullscreen();
		bool set_window_size(GLuint width, GLuint height);
		bool update_resolution(const glm::vec2& new_res);
		bool enable_vsync();
		bool disable_vsync();

		
		void clear()const;
		void present()const;
		bool render_all(const Camera_ptr& camera);

		
		bool toggle_aliasing();
		bool toggle_bloom();
		bool toggle_mouse();
		bool toggle_ambient_occlusion();
		bool toggle_shadows();
};

typedef std::shared_ptr<Renderer> Renderer_ptr;
/*=============================================*/

#endif