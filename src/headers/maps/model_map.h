#ifndef MODEL_MAP_H
#define MODEL_MAP_H

#include <unordered_map>
#include <string>
#include <vector>

const std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> ENGINE_MODELS = {
	{"cube",{"base_shader", {"cube_0", }}},
	{"nanosuit2",{"base_shader", {"nanosuit2_0", "nanosuit2_1", "nanosuit2_2", "nanosuit2_3", "nanosuit2_4", "nanosuit2_5", "nanosuit2_6", }}},
	{"ninja",{"base_shader", {"ninja_0", "ninja_1", }}},
};

#endif
