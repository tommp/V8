#ifndef IMAGE_MAP_H
#define IMAGE_MAP_H

#include <unordered_map>
#include <string>

const std::unordered_map<std::string, unsigned int> WORLD_TEXTURES = {
	{"./images/LeafBush.png", 0},
	{"./images/LeafBushSmall.png", 1},
	{"./images/SharpBush.png", 2},
	{"./images/simple.png", 3},
	{"./images/tallBentBush.png", 4},
	{"./images/tallBerryBush.png", 5},
};
#endif
