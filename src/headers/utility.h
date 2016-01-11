#ifndef UTILITY_H
#define UTILITY_H

/*Included headers*/
/*---------------------------------------------*/

/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <string>
#include <unistd.h>
#include <unordered_map>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

const unsigned int SCREEN_HEIGHT =			320;
const unsigned int SCREEN_WIDTH	=			640;
const unsigned int RENDERING_SLACK_FACTOR = 1;

const unsigned int TILESIZE = 				32;
const unsigned int SCREEN_FPS = 			60;
const int SCREEN_TICKS_PER_FRAME =			1000 / SCREEN_FPS;

const std::unordered_map<std::string, std::string> WORLD_TEXTURES = {
	{"simple", "../pixelart/Animations/simple.png"},
	{"blob", "../pixelart/Animations/blob.png"},
};

const std::unordered_map<std::string, std::string> WORLD_ANIMATIONS = {
	{"simple", "../pixelart/Animations/simple.txt"},
	{"blob", "../pixelart/Animations/blob.txt"},
};

/*Waits for user input and quits when detected*/
void wait_for_event();

/* NOT USED */
void generate_animation(const char* filename, const char* animation_name);
/*=============================================*/

#endif
