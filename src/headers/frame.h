#ifndef FRAME_H
#define FRAME_H


/*Included headers*/
/*---------------------------------------------*/
#include "./errorlogger.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/
class Animation;

class Frame{
	friend class Animation;
	private:
		/* Clipping rectangle for texture */
		SDL_Rect clip;
		/* duration in ms */
		int duration;
	public:
		Frame(int x, int y, int w, int h, int frame_duration);
};
/*=============================================*/

#endif