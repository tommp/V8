#ifndef MATERIAL_MAP_H
#define MATERIAL_MAP_H

#include <unordered_map>
#include <string>
#include <vector>

const std::unordered_map<std::string, std::vector<std::string>> ENGINE_MATERIALS = {
	{"cube_mat", {"BrickRound0105_5_S", "BrickRound0105_5_SPEC"}},
	{"floor_mat", {"BrickRound0105_5_S", ""}},
	{"Model001_Material001", {"Turret_01", ""}},
	{"Material.001_BrickRound0105_5_SPEC.png", {"BrickRound0105_5_SPEC", ""}},
};

#endif
