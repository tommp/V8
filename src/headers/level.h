#ifndef LEVEL_H
#define LEVEL_H


/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
#include "utility.h"
#include "enum_light_type.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
using std::vector;

class Tile;

class Level {
	private:
		GLuint width;
		GLuint height;
		GLuint depth;
	public:
		Level(int init_width, int init_height, int init_depth);

		GLuint get_width()const { return width; };
		GLuint get_height()const { return height; };
		GLuint get_depth()const { return depth; };

		void render_level();

		void load_level();
		void unload_level();
		void save_level();
		void generate_level();
};

typedef std::shared_ptr<Level> Level_ptr;
/*=============================================*/

#endif