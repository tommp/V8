#ifndef ANIMATION_SET_MAP_H
#define ANIMATION_SET_MAP_H

#include <unordered_map>
#include <string>
#include <vector>

/* Animation sets, name as the key, vector of animations as the value */
const std::unordered_map<std::string, std::vector<std::string>> ENGINE_ANIMATION_SETS = {
	{"player", {"player_test", }},
	{"cube_test", {"cube_walk", }},
};

#endif


