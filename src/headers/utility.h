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
/*Waits for user input and quits when detected*/
void wait_for_event();

/*Waits for user input and quits when detected*/
void generate_animation(const char* filename, const char* animation_name);
/*=============================================*/

#endif
