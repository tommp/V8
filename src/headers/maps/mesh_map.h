#ifndef MESH_MAP_H
#define MESH_MAP_H

#include <unordered_map>
#include <string>
#include <vector>

const std::unordered_map<std::string, std::pair<unsigned int, std::string>> ENGINE_MESHES = {
	{"cube_0", {0, "DefaultMaterial"}},
 	{"nanosuit2_0", {920, "Helmet"}},
 	{"nanosuit2_1", {261856, "Glass"}},
 	{"nanosuit2_2", {267480, "Glass"}},
 	{"nanosuit2_3", {270296, "Body"}},
 	{"nanosuit2_4", {569680, "Arm"}},
 	{"nanosuit2_5", {814632, "Hand"}},
 	{"nanosuit2_6", {1511240, "Leg"}},
 	{"ninja_0", {2059240, "Material0"}},
 	{"ninja_1", {2156880, "Material0"}},
 };

#endif
