#ifndef MAIN_H
#define MAIN_H


/*Included headers*/
/*---------------------------------------------*/
#include "./display.h"
#include "./resource_manager.h"
#include "./utility.h"
#include "./timer.h"
#include "./level.h"
#include "./errorlogger.h"
#include "./texture.h"
#include "./events.h"
#include "./animation.h"
#include "./player.h"

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
/*=============================================*/

#endif