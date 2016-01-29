#ifndef IMAGE_MAP_H
#define IMAGE_MAP_H

#include <unordered_map>
#include <string>

const std::unordered_map<std::string, unsigned int> ENGINE_IMAGES = {
	{"arm_dif", 0},
	{"body_dif", 1048588},
	{"glass_dif", 2097176},
	{"hand_dif", 2113572},
	{"helmet_dif", 3162160},
	{"leg_dif", 4210748},
};

#endif
