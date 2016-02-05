#ifndef MESH_MAP_H
#define MESH_MAP_H

#include <unordered_map>
#include <string>
#include <vector>

const std::unordered_map<std::string, std::pair<unsigned int, std::string>> ENGINE_MESHES = {
	{"cube_0", {0, "DefaultMaterial"}},
 	{"ninja_0", {920, "Material0"}},
 	{"ninja_1", {98560, "Material0"}},
 };

#endif
