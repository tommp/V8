#ifndef MODEL_MAP_H
#define MODEL_MAP_H

#include <unordered_map>
#include <string>
#include <vector>

const std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> ENGINE_MODELS = {
	{"cube",{"unlit_shader", {"cube_0", }}},
	{"ninja",{"unlit_shader", {"ninja_0", "ninja_1", }}},
};

#endif
