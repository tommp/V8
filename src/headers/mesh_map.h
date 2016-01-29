#ifndef MESH_MAP_H
#define MESH_MAP_H

#include <unordered_map>
#include <string>
#include <vector>

const std::unordered_map<std::string, std::pair<unsigned int, std::vector<std::string>>> WORLD_MESHES = {
	{"nanosuit2_0", {0, {"helmet_d", }}},
	{"nanosuit2_1", {376904, {"glass_di", }}},
	{"nanosuit2_2", {385024, {"glass_di", }}},
	{"nanosuit2_3", {389088, {"body_dif", }}},
	{"nanosuit2_4", {821528, {"arm_dif", }}},
	{"nanosuit2_5", {1175344, {"hand_dif", }}},
	{"nanosuit2_6", {2181552, {"leg_dif", }}},
};

#endif
