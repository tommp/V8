#ifndef WINDOW_H
#define WINDOW_H

/*Included headers*/
/*---------------------------------------------*/
#include "./errorlogger.h"
#include "./utility.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <fstream>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
#define DISPLAY_SETTINGS_FILE_PATH "./data/display.conf"
#define OPENGL_MAJOR_VERSION 2
#define OPENGL_MINOR_VERSION 1

class Display{
	private:
		SDL_Window* window;
		SDL_GLContext gl_context;

	    bool use_vsync;
	    bool use_fullscreen;
	    bool mouse_visible;

	    GLint width;
	    GLint height;
	    GLint screen_width;
	    GLint screen_height;
	public:
		Display();
		bool save_settings();
		bool load_settings();
		void toggle_mouse();
		bool init_window();
		bool init_openGL();
		bool enable_fullscreen();
		bool enable_vsync();
		bool disable_vsync();

		void clear(){glClear(GL_COLOR_BUFFER_BIT);};
		void present(){SDL_GL_SwapWindow(window);};

		GLuint get_screen_width(){return screen_width;};
		GLuint get_screen_height(){return screen_height;};
		GLuint get_width(){return width;};
		GLuint get_height(){return height;};
};
/*=============================================*/

#endif