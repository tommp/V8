#ifndef MESH_MAP_H
#define MESH_MAP_H

#include <unordered_map>
#include <string>
#include <vector>

const std::unordered_map<std::string, std::pair<unsigned int, std::vector<std::string>>> ENGINE_MESHES = {
	{"cube_0", {0, {}}},
	{"nanosuit2_0", {1304, {"helmet_d", }}},
	{"nanosuit2_1", {378208, {"glass_di", }}},
	{"nanosuit2_2", {386328, {"glass_di", }}},
	{"nanosuit2_3", {390392, {"body_dif", }}},
	{"nanosuit2_4", {822832, {"arm_dif", }}},
	{"nanosuit2_5", {1176648, {"hand_dif", }}},
	{"nanosuit2_6", {2182856, {"leg_dif", }}},
	{"ninja_0", {2974408, {"nskinbl", }}},
	{"ninja_1", {3115440, {"nskinbl", }}},
};

#endif
