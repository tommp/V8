#ifndef LEVEL_H
#define LEVEL_H


/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
#include "utility.h"
#include "actor.h"
#include "tiles.h"
#include "display.h"
#include "light.h"
#include "camera.h"
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

class Level {
	private:
		int width;
		int height;
		int depth;

		Camera camera;
		vector<vector<vector<Tile>>> level_tiles;
		vector<Light> lights;

	public:
		Level(int init_width, int init_height, int init_depth);

		int get_width()const { return width; };
		int get_height()const { return height; };
		int get_depth()const { return depth; };

		void center_camera(const Actor_ptr target);
		void render_level();
		Camera* get_camera_pointer(){ return &camera; };

		void load_level();
		void unload_level();
		void save_level();
		void generate_level();
};

typedef std::shared_ptr<Level> Level_ptr;
/*=============================================*/

#endif