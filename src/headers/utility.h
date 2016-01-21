#ifndef UTILITY_H
#define UTILITY_H

/*Included headers*/
/*---------------------------------------------*/
#include "./errorlogger.h"
/*---------------------------------------------*/

/*Included dependencies*/
/*---------------------------------------------*/
#include <SDL2/SDL.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <unordered_map>
/*---------------------------------------------*/

/*Header content*/
/*=============================================*/

/* STATIC LINKAGE, CHANGE TO DYNAMIC LATER */
#define DISABLE_VSYNC 0
#define IMAGE_DATA_PATH "./data/content.blue"
#define IMAGE_DATA_INDEX_PATH "./data/register.blue"

const unsigned int SCREEN_HEIGHT =			320;
const unsigned int SCREEN_WIDTH	=			640;
const int RENDERING_SLACK = 				100;


const unsigned int TILESIZE = 				32;
const unsigned int SCREEN_FPS = 			60;
const int SCREEN_TICKS_PER_FRAME =			1000 / SCREEN_FPS;


/* World textures, the name as the key, path as the value */
const std::unordered_map<std::string, std::string> WORLD_TEXTURES = {
	{"simple", "../pixelart/Animations/simple.png"},
	{"blob", "../pixelart/Animations/blob.png"},
	{"adv_up", "../pixelart/Animations/adventure_up.png"},
	{"adv_down", "../pixelart/Animations/adventure_down.png"},
	{"adv_left", "../pixelart/Animations/adventure_left.png"},
	{"adv_right", "../pixelart/Animations/adventure_right.png"},
	{"adv_up_loiter", "../pixelart/Animations/adventure_up.png"},
	{"adv_down_loiter", "../pixelart/Animations/adventure_down.png"},
	{"adv_left_loiter", "../pixelart/Animations/adventure_left.png"},
	{"adv_right_loiter", "../pixelart/Animations/adventure_right.png"},
};

/* World animations, name as the key, texture name and animation text file separated by : as the value */
const std::unordered_map<std::string, std::string> WORLD_ANIMATIONS = {
	{"simple", "../pixelart/Animations/simple.txt"},
	{"blob", "../pixelart/Animations/blob.txt"},
	{"adv_up", "../pixelart/Animations/adventure.txt"},
	{"adv_down", "../pixelart/Animations/adventure.txt"},
	{"adv_left", "../pixelart/Animations/adventure.txt"},
	{"adv_right", "../pixelart/Animations/adventure.txt"},
	{"adv_up_loiter", "../pixelart/Animations/adventure_loiter.txt"},
	{"adv_down_loiter", "../pixelart/Animations/adventure_loiter.txt"},
	{"adv_left_loiter", "../pixelart/Animations/adventure_loiter.txt"},
	{"adv_right_loiter", "../pixelart/Animations/adventure_loiter.txt"},
};

const std::unordered_map<std::string, std::string> WORLD_ANIMATION_SETS = {
	{"adventure_time", "../pixelart/Animations/adventure_time.txt"},
	{"blob_movement", "../pixelart/Animations/blob_movement.txt"},
};

/* Waits for user input and quits when detected */
void wait_for_event();
/*=============================================*/

#endif
