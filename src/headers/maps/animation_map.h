#ifndef ANIMATION_MAP_H
#define ANIMATION_MAP_H

#include <GL/glew.h>
#include <unordered_map>
#include <vector>
#include <string>

/* World animations, name as the key, file index and model names as the value */
const std::unordered_map<std::string, std::pair<GLuint, std::vector<std::string>>> ENGINE_ANIMATIONS = {
	{"cube_walk", {0, {"engine_base_cube", }}},
	{"player_test", {5, {"bob_lamp_update", }}},
	{"player_walk", {10, {"bob_lamp_update", }}},
	{"prop", {15, {"engine_base_floor", }}},
};

#endif
