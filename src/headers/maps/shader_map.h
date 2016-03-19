#ifndef SHADER_MAP_H
#define SHADER_MAP_H

#include <unordered_map>
#include <string>

const std::unordered_map<std::string, std::pair<std::string, std::string>> WORLD_SHADERS = {
	{"geometry_shader_static", {"vertex_geometry_static.vs", "fragment_geometry_static.fs"}},
	{"geometry_shader_static_colored", {"vertex_geometry_static_colored.vs", "fragment_geometry_static_colored.fs"}},
	{"geometry_shader_animated", {"vertex_geometry_animated.vs", "fragment_geometry_animated.fs"}},
	{"geometry_shader_animated_colored", {"vertex_geometry_animated_colored.vs", "fragment_geometry_animated_colored.fs"}},
	{"point_light_shader", {"vertex_point_light.vs", "fragment_point_light.fs"}},
	{"spot_light_shader", {"vertex_spot_light.vs", "fragment_spot_light.fs"}},
	{"dir_light_shader", {"vertex_dir_light.vs", "fragment_dir_light.fs"}},
	{"vertical_blur_shader", {"vertex_blur.vs", "fragment_vertical_blur.fs"}},
	{"horizontal_blur_shader", {"vertex_blur.vs", "fragment_horizontal_blur.fs"}},
	{"bloom_shader", {"vertex_blur.vs", "fragment_bloom.fs"}},
};

#endif




