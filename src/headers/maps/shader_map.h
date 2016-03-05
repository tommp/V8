#ifndef SHADER_MAP_H
#define SHADER_MAP_H

#include <unordered_map>
#include <string>

const std::unordered_map<std::string, std::pair<std::string, std::string>> WORLD_SHADERS = {
	{"base_shader", {"vertex_base.vs", "fragment_base.fs"}},
	{"test_shader", {"vertex_test.vs", "fragment_test.fs"}},
	{"lit_shader", {"vertex_lit.vs", "fragment_lit.fs"}},
	{"unlit_shader", {"vertex_unlit.vs", "fragment_unlit.fs"}},
	{"geometry_shader", {"vertex_geometry.vs", "fragment_geometry.fs"}},
	{"light_shader", {"vertex_light.vs", "fragment_light.fs"}},
	{"point_light_shader", {"vertex_point_light.vs", "fragment_point_light.fs"}},
	{"spot_light_shader", {"vertex_spot_light.vs", "fragment_spot_light.fs"}},
	{"dir_light_shader", {"vertex_dir_light.vs", "fragment_dir_light.fs"}},
	{"shadow_shader", {"vertex_shadow.vs", "fragment_shadow.fs"}},
};

#endif




