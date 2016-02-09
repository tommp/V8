#ifndef MODEL_MAP_H
#define MODEL_MAP_H

#include <unordered_map>
#include <string>
#include <vector>

const std::unordered_map<std::string, std::vector<std::string>> ENGINE_MODELS = {
	{"cube", {"cube_0", }},
	{"ninja", {"ninja_0", "ninja_1", }},
};

#endif
