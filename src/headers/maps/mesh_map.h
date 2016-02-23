#ifndef MESH_MAP_H
#define MESH_MAP_H

#include <unordered_map>
#include <string>
#include <vector>

const std::unordered_map<std::string, std::pair<unsigned int, std::string>> ENGINE_MESHES = {
	{"brick_box_0", {0, "cube_mat"}},
 	{"floor_base_0", {920, "floor_mat"}},
 	{"portalturret_0", {1080, "Model001_Material001"}},
 	{"untitled_0", {664100, "Material.001_BrickRound0105_5_SPEC.png"}},
 };

#endif
