#ifndef SHADER_MAP_H
#define SHADER_MAP_H

#include <unordered_map>
#include <string>

const std::unordered_map<std::string, std::pair<std::string, std::string>> WORLD_SHADERS = {
	{"base_shader", {"../data/shaders/vertex_base.vs", "../data/shaders/fragment_base.fs"}},
	{"test_shader", {"../data/shaders/vertex_test.vs", "../data/shaders/fragment_test.fs"}},
	{"lit_shader", {"../data/shaders/vertex_lit.vs", "../data/shaders/fragment_lit.fs"}},
	{"unlit_shader", {"../data/shaders/vertex_unlit.vs", "../data/shaders/fragment_unlit.fs"}},
	{"geometry_shader", {"../data/shaders/vertex_geometry.vs", "../data/shaders/fragment_geometry.fs"}},
	{"light_shader", {"../data/shaders/vertex_light.vs", "../data/shaders/fragment_light.fs"}},
};

#endif




