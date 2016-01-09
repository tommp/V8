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

/*Waits for user input and quits when detected*/
void wait_for_event();

/* NOT USED */
void generate_animation(const char* filename, const char* animation_name);
/*=============================================*/

#endif
