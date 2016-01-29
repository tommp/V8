#ifndef ANIMATION_SET_MAP_H
#define ANIMATION_SET_MAP_H

#include <unordered_map>
#include <string>
#include <vector>

const std::unordered_map<std::string, std::vector<std::string>> WORLD_ANIMATION_SETS = {
	{"player_test", {"player_walk", "player_fire"}},
	{"cube_test", {"cube_walk"}},
};

#endif


