#ifndef SHADER_MAP_H
#define SHADER_MAP_H

#include <unordered_map>
#include <string>

const std::unordered_map<std::string, std::pair<std::string, std::string>> WORLD_SHADERS = {
	{"base_shader", {"./src/shaders/vertex_base.vs", "./src/shaders/fragment_base.fs"}},
	{"test_shader", {"./src/shaders/vertex_test.vs", "./src/shaders/fragment_test.fs"}},
	{"lit_shader", {"./src/shaders/vertex_lit.vs", "./src/shaders/fragment_lit.fs"}},
	{"unlit_shader", {"./src/shaders/vertex_unlit.vs", "./src/shaders/fragment_unlit.fs"}},
	{"geometry_shader", {"./src/shaders/vertex_geometry.vs", "./src/shaders/fragment_geometry.fs"}},
	{"light_shader", {"./src/shaders/vertex_light.vs", "../src/shaders/fragment_light.fs"}},
	{"point_light_shader", {"./src/shaders/vertex_point_light.vs", "./src/shaders/fragment_point_light.fs"}},
	{"spot_light_shader", {"./src/shaders/vertex_spot_light.vs", "./src/shaders/fragment_spot_light.fs"}},
	{"dir_light_shader", {"./src/shaders/vertex_dir_light.vs", "./src/shaders/fragment_dir_light.fs"}},
};

#endif




