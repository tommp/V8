#ifndef MATERIAL_MAP_H
#define MATERIAL_MAP_H

#include <unordered_map>
#include <string>
#include <vector>

const std::unordered_map<std::string, std::vector<std::string>> ENGINE_MATERIALS = {
	{"DefaultMaterial", {"", "", ""}},
	{"Helmet", {"helmet_d", "", ""}},
	{"Glass", {"glass_di", "", ""}},
	{"Glass", {"glass_di", "", ""}},
	{"Body", {"body_dif", "", ""}},
	{"Arm", {"arm_dif", "", ""}},
	{"Hand", {"hand_dif", "", ""}},
	{"Leg", {"leg_dif", "", ""}},
	{"Material0", {"nskinbl", "", ""}},
	{"Material0", {"nskinbl", "", ""}},
};

#endif
