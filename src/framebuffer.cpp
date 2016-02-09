#include "framebuffer.h"

G_Framebuffer::G_Framebuffer(){

}

void G_Framebuffer::load_framebuffer(GLuint screen_width, GLuint screen_height){
	/* Initialize framebuffer */
	glGenFramebuffers(1, &g_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
	  
	// - Position color buffer
	glGenTextures(1, &g_position);
	glBindTexture(GL_TEXTURE_2D, g_position);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_position, 0);
	  
	// - Normal color buffer
	glGenTextures(1, &g_normal);
	glBindTexture(GL_TEXTURE_2D, g_normal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_normal, 0);
	  
	// - Color + Specular color buffer
	glGenTextures(1, &g_albedo_spec);
	glBindTexture(GL_TEXTURE_2D, g_albedo_spec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_albedo_spec, 0);
	
	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	glGenRenderbuffers(1, &rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_width, screen_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
	 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		errorlogger( "ERROR: Framebuffer not complete!");
		print_framebuffer_error_in_fucking_english();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void G_Framebuffer::bind_buffer_data(const Shader_ptr& shader){
	shader->use();
	glUniform1i(glGetUniformLocation(shader->get_program(), "g_position"), 0);
	glUniform1i(glGetUniformLocation(shader->get_program(), "g_normal"), 1);
	glUniform1i(glGetUniformLocation(shader->get_program(), "g_albedo_spec"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_position);
	
	if(check_ogl_error()) {
		std::cout << "ERROR: Failed to bind g_position: " << std::endl;
	}
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, g_normal);
	if(check_ogl_error()) {
		std::cout << "ERROR: Failed to bind g_normal: " << std::endl;
	}
	
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, g_albedo_spec);
	if(check_ogl_error()) {
		std::cout << "ERROR: Failed to bind g_albedo_spec: " << std::endl;
	}
	
	
}