#ifndef UTILITY_H
#define UTILITY_H

/*Included headers*/
/*---------------------------------------------*/

/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

#define SCREEN_HEIGHT	640;
#define SCREEN_WIDTH	360;
const unsigned int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

/*Waits for user input and quits when detected*/
void wait_for_event();

/* NOT USED */
void generate_animation(const char* filename, const char* animation_name);
/*=============================================*/

#endif
