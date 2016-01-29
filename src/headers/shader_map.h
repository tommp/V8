#ifndef SHADER_MAP_H
#define SHADER_MAP_H

#include <unordered_map>
#include <string>

const std::unordered_map<std::string, std::pair<std::string, std::string>> WORLD_SHADERS = {
	{"test_shader", {"./data/shaders/vertex_base.vs", "./data/shaders/fragment_base.fs"}},
};

#endif




