#ifndef MAIN_H
#define MAIN_H


/*Included headers*/
/*---------------------------------------------*/
#include "./utility.h"
#include "./timer.h"
#include "./level.h"
#include "./errorlogger.h"
#include "./texture.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <unistd.h>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

const unsigned int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

const unsigned int WIDTH = 640;
const unsigned int HEIGHT = 480;

/*=============================================*/

#endif