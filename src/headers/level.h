#ifndef LEVEL_H
#define LEVEL_H


/*Included headers*/
/*---------------------------------------------*/
#include "./utility.h"
#include "./actor.h"
#include "./tiles.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Actor;

using std::vector;

namespace level {
	const int cam_width = SCREEN_WIDTH;
	const int cam_height = SCREEN_HEIGHT;
}

class Level {
	private:
		int width;
		int height;
		int depth;
		SDL_Rect camera;
		vector<vector<vector<Tile>>> level_tiles;

	public:
		Level( int init_width, int init_height, int init_depth );

		int get_width()const { return width; };
		int get_height()const { return height; };
		int get_depth()const { return depth; };

		void center_camera(const Actor& target);

		SDL_Rect* get_camera_refrence(){ return &camera; };

		void load_level();
		void unload_level();
		void save_level();
		void generate_level();
};

typedef std::shared_ptr<Level> Level_ptr;
/*=============================================*/

#endif