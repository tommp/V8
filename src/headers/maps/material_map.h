#ifndef MATERIAL_MAP_H
#define MATERIAL_MAP_H

#include <unordered_map>
#include <string>
#include <vector>

const std::unordered_map<std::string, std::vector<std::string>> ENGINE_MATERIALS = {
	{"DefaultMaterial", {"", ""}},
	{"Material__4", {"c001_07", ""}},
	{"Material__5", {"c001_11", ""}},
	{"Material__3", {"c001_11", ""}},
	{"Material__7", {"c001_13", ""}},
	{"Material__6", {"c001_16", ""}},
	{"Material__2", {"c001_02", ""}},
	{"Material__25", {"w001_00", ""}},
	{"Model001_Material001", {"Turret_01", ""}},
	{"Material.001_BrickRound0105_5_SPEC.png", {"BrickRound0105_5_SPEC", ""}},
};

#endif
