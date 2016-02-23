#ifndef ANIMATION_MAP_H
#define ANIMATION_MAP_H

#include <GL/glew.h>
#include <unordered_map>
#include <vector>
#include <string>

/* World animations, name as the key, file index and model names as the value */
const std::unordered_map<std::string, std::pair<GLuint, std::vector<std::string>>> ENGINE_ANIMATIONS = {
	{"cube_walk", {0, {"portalturret", }}},
	{"player_test", {5, {"portalturret", }}},
	{"player_walk", {10, {"portalturret", }}},
	{"prop", {15, {"brick_box", }}},
};

#endif
