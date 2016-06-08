#ifndef TERRAIN_H
#define TERRAIN_H


/*Included headers*/
/*---------------------------------------------*/
#include "errorlogger.h"
#include "utility.h"
#include "shader.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <GL/glew.h>
#include <string>
#include <memory>
#include <cstdlib>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

class MC_Terrain{
	private:
		
	public:
		MC_Terrain();
		~MC_Terrain();

};

typedef std::shared_ptr<MC_Terrain> MC_Terrain_ptr;
/*=============================================*/
#endif