#ifndef IMAGE_MAP_H
#define IMAGE_MAP_H

#include <unordered_map>
#include <string>

const std::unordered_map<std::string, unsigned int> WORLD_IMAGES = {
	{"LeafBush", 0},
	{"LeafBushSmall", 4108},
	{"SharpBush", 8216},
	{"adventure_down", 12324},
	{"adventure_left", 22576},
	{"adventure_right", 32828},
	{"adventure_up", 43080},
	{"blob", 53332},
	{"simple", 61536},
	{"tallBentBush", 71788},
	{"tallBerryBush", 75896},
};

#endif
