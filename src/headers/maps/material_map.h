#ifndef MATERIAL_MAP_H
#define MATERIAL_MAP_H

#include <unordered_map>
#include <string>
#include <vector>

const std::unordered_map<std::string, std::pair<std::string, std::vector<std::string>>> ENGINE_MATERIALS = {
	{"DefaultMaterial", {"geometry_shader", {"nskinbl", "nskinbl"}}},
	{"Material0", {"geometry_shader", {"nskinbl", "nskinbl"}}},
};

#endif
