#ifndef TILE_H
#define TILE_H
/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <GL/glew.h>
#include <vector>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Tile{
private:
	std::vector<GLuint> dir_light_indices;
	std::vector<GLuint> point_light_indices;
	std::vector<GLuint> spot_light_indices;
	GLuint num_lights;
public:
	Tile();
	void increment_num_lights();

};



#endif