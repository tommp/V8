#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H


/*Included headers*/
/*---------------------------------------------*/
#include "glm.h"
#include "errorlogger.h"
#include "utility.h"
#include "shader.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
/* TODO: ADD SINGLETON STUFF LATER */
class G_Framebuffer{
private:
	GLuint g_buffer;

	GLuint g_position;
	GLuint g_normal;
	GLuint g_albedo_spec;
	GLuint rbo_depth;
public:
	G_Framebuffer();
	void load_framebuffer(GLuint screen_width, GLuint screen_height);
	void use(){glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);};
	void disable(){glBindFramebuffer(GL_FRAMEBUFFER, 0);};
	void bind_buffer_data(const Shader_ptr& shader);
	GLuint get_buffer(){return g_buffer;};
};
/*=============================================*/

#endif