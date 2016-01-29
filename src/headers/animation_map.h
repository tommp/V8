#ifndef ANIMATION_MAP_H
#define ANIMATION_MAP_H

#include <GL/glew.h>
#include <unordered_map>
#include <string>

/* World animations, name as the key, file index and texture name as the value */
const std::unordered_map<std::string, std::pair<GLuint, std::string>> WORLD_ANIMATIONS = {
	{"simple", {0, "simple"}},
};

#endif


